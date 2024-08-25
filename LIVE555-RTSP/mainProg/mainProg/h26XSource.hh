/*
	Base on DeviceSource.hh
*/

#ifndef __H26XSOURCE_HH
#define __H26XSOURCE_HH

#ifndef _FRAMED_SOURCE_HH
#include "FramedSource.hh"
#endif

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

#include "avSource.h"

typedef VVTK_RET_CALLBACK (*onVideoSampleCapture)(const vvtk_video_frame_t *videoFrame, const void *arg);

class H26XSource : public FramedSource
{
public:
	static H26XSource *createNew(UsageEnvironment &env);

public:
	static EventTriggerId eventTriggerId;
	// Note that this is defined here to be a static class variable, because this code is intended to illustrate how to
	// encapsulate a *single* device - not a set of devices.
	// You can, however, redefine this to be a non-static member variable.
	void doSignalNewFrameData();

protected:
	H26XSource(UsageEnvironment &env);
	// called only by createNew(), or by subclass constructors
	virtual ~H26XSource();

private:
	// redefined virtual functions:
	virtual void doGetNextFrame();
	virtual void doStopGettingFrames();

private:
	static void deliverFrame0(void *clientData);
	void deliverFrame();

private:
	static unsigned referenceCount; // used to count how many instances of this class currently exist
};

#endif /* __H26XSOURCE_HH */
