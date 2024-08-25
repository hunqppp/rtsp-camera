/*
    Base on H264VideoFileServerMediaSubsession.hh
*/

#ifndef _H264_VIDEO_SOURCE_SERVER_MEDIA_SUBSESSION_HH
#define _H264_VIDEO_SOURCE_SERVER_MEDIA_SUBSESSION_HH

#ifndef _FILE_SERVER_MEDIA_SUBSESSION_HH
#include "FileServerMediaSubsession.hh"
#endif

class H264VideoSourceServerMediaSubsession : public FileServerMediaSubsession
{
public:
    static H264VideoSourceServerMediaSubsession *
    createNew(UsageEnvironment &env, char const *fileName, Boolean reuseFirstSource);

    // Used to implement "getAuxSDPLine()":
    void checkForAuxSDPLine1();
    void afterPlayingDummy1();

protected:
    H264VideoSourceServerMediaSubsession(UsageEnvironment &env,
                                         char const *fileName, Boolean reuseFirstSource);
    // called only by createNew();
    virtual ~H264VideoSourceServerMediaSubsession();

    void setDoneFlag() { fDoneFlag = ~0; }

protected: // redefined virtual functions
    virtual char const *getAuxSDPLine(RTPSink *rtpSink,
                                      FramedSource *inputSource);
    virtual FramedSource *createNewStreamSource(unsigned clientSessionId,
                                                unsigned &estBitrate);
    virtual RTPSink *createNewRTPSink(Groupsock *rtpGroupsock,
                                      unsigned char rtpPayloadTypeIfDynamic,
                                      FramedSource *inputSource);

private:
    char *fAuxSDPLine;
    char fDoneFlag;         // used when setting up "fAuxSDPLine"
    RTPSink *fDummyRTPSink; // ditto
};

#endif
