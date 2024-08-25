#include "H264VideoSourceServerMediaSubsession.hh"
#include "H264VideoRTPSink.hh"
#include "ByteStreamFileSource.hh"
#include "H264VideoStreamFramer.hh"
#include "h26XSource.hh"

H264VideoSourceServerMediaSubsession *
H264VideoSourceServerMediaSubsession::createNew(UsageEnvironment &env,
                                                char const *fileName,
                                                Boolean reuseFirstSource)
{
    return new H264VideoSourceServerMediaSubsession(env, fileName, reuseFirstSource);
}

H264VideoSourceServerMediaSubsession::H264VideoSourceServerMediaSubsession(UsageEnvironment &env,
                                                                           char const *fileName, Boolean reuseFirstSource)
    : FileServerMediaSubsession(env, fileName, reuseFirstSource),
      fAuxSDPLine(NULL), fDoneFlag(0), fDummyRTPSink(NULL)
{
}

H264VideoSourceServerMediaSubsession::~H264VideoSourceServerMediaSubsession()
{
    delete[] fAuxSDPLine;
}

static void afterPlayingDummy(void *clientData)
{
    H264VideoSourceServerMediaSubsession *subsess = (H264VideoSourceServerMediaSubsession *)clientData;
    subsess->afterPlayingDummy1();
}

void H264VideoSourceServerMediaSubsession::afterPlayingDummy1()
{
    // Unschedule any pending 'checking' task:
    envir().taskScheduler().unscheduleDelayedTask(nextTask());
    // Signal the event loop that we're done:
    setDoneFlag();
}

static void checkForAuxSDPLine(void *clientData)
{
    H264VideoSourceServerMediaSubsession *subsess = (H264VideoSourceServerMediaSubsession *)clientData;
    subsess->checkForAuxSDPLine1();
}

void H264VideoSourceServerMediaSubsession::checkForAuxSDPLine1()
{
    nextTask() = NULL;

    char const *dasl;
    if (fAuxSDPLine != NULL)
    {
        // Signal the event loop that we're done:
        setDoneFlag();
    }
    else if (fDummyRTPSink != NULL && (dasl = fDummyRTPSink->auxSDPLine()) != NULL)
    {
        fAuxSDPLine = strDup(dasl);
        fDummyRTPSink = NULL;

        // Signal the event loop that we're done:
        setDoneFlag();
    }
    else if (!fDoneFlag)
    {
        // try again after a brief delay:
        int uSecsToDelay = 100000; // 100 ms
        nextTask() = envir().taskScheduler().scheduleDelayedTask(uSecsToDelay,
                                                                 (TaskFunc *)checkForAuxSDPLine, this);
    }
}

char const *H264VideoSourceServerMediaSubsession::getAuxSDPLine(RTPSink *rtpSink, FramedSource *inputSource)
{
    if (fAuxSDPLine != NULL)
        return fAuxSDPLine; // it's already been set up (for a previous client)

    if (fDummyRTPSink == NULL)
    { // we're not already setting it up for another, concurrent stream
        // Note: For H264 video files, the 'config' information ("profile-level-id" and "sprop-parameter-sets") isn't known
        // until we start reading the file.  This means that "rtpSink"s "auxSDPLine()" will be NULL initially,
        // and we need to start reading data from our file until this changes.
        fDummyRTPSink = rtpSink;

        // Start reading the file:
        fDummyRTPSink->startPlaying(*inputSource, afterPlayingDummy, this);

        // Check whether the sink's 'auxSDPLine()' is ready:
        checkForAuxSDPLine(this);
    }

    envir().taskScheduler().doEventLoop(&fDoneFlag);

    return fAuxSDPLine;
}
FramedSource *H264VideoSourceServerMediaSubsession::createNewStreamSource(unsigned /*clientSessionId*/, unsigned &estBitrate)
{
    // estBitrate = 500; // kbps, estimate
    // estBitrate = 1024 * 1024;

    H26XSource *h26xSource = H26XSource::createNew(envir());
    if (h26xSource == NULL)
    {
        envir() << "Unable to read from\"" << "Buffer" << "\" as a byte-stream source\n";
        exit(1);
    }

    FramedSource *videoSource = h26xSource;

    return H264VideoStreamFramer::createNew(envir(), videoSource);
}

RTPSink *H264VideoSourceServerMediaSubsession ::createNewRTPSink(Groupsock *rtpGroupsock,
                                                                 unsigned char rtpPayloadTypeIfDynamic,
                                                                 FramedSource * /*inputSource*/)
{
    return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
}
