#include <csignal>
#include <unistd.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "main.h"
#include "avSource.h"
#include "libsupport/Utils.h"

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "announceURL.hh"
#include "h26XSource.hh"
#include "H264VideoSourceServerMediaSubsession.hh"
#include "LiveServerMediaSubsession.hh"

using namespace std;
using json = nlohmann::json;

UsageEnvironment* env;

// To make the second and subsequent client for each stream reuse the same
// input stream as the first client (rather than playing the file from the
// start for each client), change the following "False" to "True":
Boolean reuseFirstSource = True;

// To stream *only* MPEG-1 or 2 video "I" frames
// (e.g., to reduce network bandwidth),
// change the following "False" to "True":
Boolean iFramesOnly = False;

/* RTSP Username & Password */
static std::string rtspUser = "";
static std::string rtspPass = "";

static H26XSource *H264FrameSource = NULL;

static void appSafeExit(int signal);
static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms, char const* streamName, char const* inputFileName);
static void getRTSPUserPassword();

int main() {
    signal(SIGINT,  appSafeExit);
	signal(SIGQUIT, appSafeExit);
	signal(SIGTERM, appSafeExit);
	signal(SIGKILL, appSafeExit);

	avSourceBegin();
	return 0;
	// avSourceOpenSocketSend();
	// getRTSPUserPassword();

    /* BEGIN CODE SEGMENT */
	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	env = BasicUsageEnvironment::createNew(*scheduler);

	UserAuthenticationDatabase* authDB = NULL;

	// To implement client access control to the RTSP server, do the following:
	if (!rtspUser.empty() && !rtspPass.empty()) {
		authDB = new UserAuthenticationDatabase;
		printf("Set RTSP username & password\n");
		authDB->addUserRecord(rtspUser.c_str(), rtspPass.c_str());
	}
	// Repeat the above with each <username>, <password> that you wish to allow
	// access to the server.

  // Create the RTSP server:
#ifdef SERVER_USE_TLS
  // Serve RTSPS: RTSP over a TLS connection:
	RTSPServer* rtspServer = RTSPServer::createNew(*env, 322, authDB);
#else
  // Serve regular RTSP (over a TCP connection):
  RTSPServer* rtspServer = NULL;

  do {
	rtspServer = RTSPServer::createNew(*env, 8554, authDB);
	if (rtspServer != NULL) {
		break;
	}
	sleep(1);
  } while (1);
	
#endif
	
#ifdef SERVER_USE_TLS
#ifndef STREAM_USING_SRTP
#define STREAM_USING_SRTP True
#endif
	rtspServer->setTLSState(PATHNAME_TO_CERTIFICATE_FILE,
		PATHNAME_TO_PRIVATE_KEY_FILE, STREAM_USING_SRTP);
#endif

	char const* descriptionString = "Session streamed by \"testOnDemandRTSPServer\"";

	// Set up each of the possible streams that can be served by the
	// RTSP server.  Each such stream is implemented using a
	// "ServerMediaSession" object, plus one or more
	// "ServerMediaSubsession" objects for each audio/video substream.

	// A H.264 video elementary stream:
	{
		char const* streamName = "profile1";
		char const* inputH264File = "test.264";
		char const* intputWAVFile = "audio.wav";
		Boolean convertToULaw = False;

		ServerMediaSession* sms = ServerMediaSession::createNew(*env, streamName, streamName, descriptionString);

	#if 1
		sms->addSubsession(H264VideoSourceServerMediaSubsession::createNew(*env,inputH264File, reuseFirstSource));
		// sms->addSubsession(WAVAudioFileServerMediaSubsession::createNew(*env, intputWAVFile, reuseFirstSource, convertToULaw));
	#else
		H264FrameSource = H26XSource::createNew(*env);
		StreamReplicator *devSource = StreamReplicator::createNew(*env, H264FrameSource, false);
		sms->addSubsession(LiveServerMediaSubsession::createNew(*env, devSource));
	#endif
		
		rtspServer->addServerMediaSession(sms);

		announceStream(rtspServer, sms, streamName, inputH264File);
	}

	// Also, attempt to create a HTTP server for RTSP-over-HTTP tunneling.
	// Try first with the default HTTP port (80), and then with the alternative
	// HTTP port numbers (8000 and 8080).

#ifdef SERVER_USE_TLS
  // (Attempt to) use the default HTTPS port (443) instead:
	char const* httpProtocolStr = "HTTPS";
	if (rtspServer->setUpTunnelingOverHTTP(443)) {
#else
	char const* httpProtocolStr = "HTTP";
	if (rtspServer->setUpTunnelingOverHTTP(80) ||
		rtspServer->setUpTunnelingOverHTTP(8000) ||
		rtspServer->setUpTunnelingOverHTTP(8080)) {
#endif
		* env << "\n(We use port " << rtspServer->httpServerPortNum()
			<< " for optional RTSP-over-" << httpProtocolStr << " tunneling.)\n";
	}
	else {
		*env << "\n(RTSP-over-" << httpProtocolStr
			<< " tunneling is not available.)\n";
	}

	env->taskScheduler().doEventLoop(); // does not return


    return 0;
}

void appSafeExit(int signal) {
    {
        (void)signal;
    }
    exit(EXIT_SUCCESS);
}

void announceStream(RTSPServer * rtspServer, ServerMediaSession * sms,
	char const* streamName, char const* inputFileName) {
	UsageEnvironment& env = rtspServer->envir();

	env << "\n\"" << streamName << "\" stream, from the file \"" << inputFileName << "\"\n";
	announceURL(rtspServer, sms);
}

void getRTSPUserPassword() {
	/*
		{
			"account": 	"rtsp",
			"password": "12345"
		}
	*/
	const std::string url = "/usr/conf/rtsp_account.json";
	json JSON;

	if (readJSONFile(JSON, url)) {
		try {
			rtspUser = JSON["account"].get<std::string>();
			rtspPass = JSON["password"].get<std::string>();
		}
		catch (const std::exception &e) {
			std::cout << e.what() << std::endl;
		}
	}

	printf("RTSP User: %s\n", rtspUser.c_str());
	printf("RTSP Pass: %s\n", rtspPass.c_str());
}