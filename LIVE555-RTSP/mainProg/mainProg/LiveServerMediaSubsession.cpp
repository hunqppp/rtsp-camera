#include "LiveServerMediaSubsession.hh"

LiveServerMediaSubsession *LiveServerMediaSubsession::createNew(UsageEnvironment &env, StreamReplicator *replicator)
{
    return new LiveServerMediaSubsession(env, replicator);
}

FramedSource *LiveServerMediaSubsession::createNewStreamSource(unsigned clientSessionId, unsigned &estBitrate)
{
    estBitrate = 9000000;
    FramedSource *source = m_replicator->createStreamReplica();

#if 0
    return H264VideoStreamDiscreteFramer::createNew(envir(), source); /* This line isn't smooth */
#else
    return H264VideoStreamFramer::createNew(envir(), source);
#endif
}

RTPSink *LiveServerMediaSubsession::createNewRTPSink(Groupsock *rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource *inputSource)
{
    return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
}
