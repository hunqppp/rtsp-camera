#include <GroupsockHelper.hh>

#include "libsupport/h26XParser.h"
#include "h26XSource.hh"
#include "avSource.h"
#include "H26XFrame.hh"

VVTK_RET_CALLBACK videoSampleCapture(const vvtk_video_frame_t *videoFrame, const void *arg) {
    H26XSource *h26xSource = (H26XSource*)arg;

	if (videoFrame->size) {
#if 0
        uint8_t *Frame = videoFrame->data + 4;
        uint32_t FrameSize = videoFrame->size - 4;
#else
        uint8_t *Frame = videoFrame->data;
        uint32_t FrameSize = videoFrame->size;
#endif
		pthread_mutex_lock(&RWQueueMutex);

#if 0
        uint32_t cursor = 0;

        while (H264FrameQueue.size() >= QueueCapacity) {
			H26XFrame *h26xFrame = H264FrameQueue.front();
			delete h26xFrame;
			H264FrameQueue.pop();
		}

        while (true) {
            auto nalu = H26X_ReadOneNaluFromBuffer(Frame, FrameSize, &cursor);
            if (nalu.empty() == false) {
                H26XFrame *h264Frame = new H26XFrame();
                h264Frame->FrameSize = FrameSize;
                h264Frame->Frame = (uint8_t*)malloc(h264Frame->FrameSize);
                memcpy(h264Frame->Frame, nalu.data(), nalu.size());

                H264FrameQueue.push(h264Frame);
            }
            else break;
        }
#else
		while (H264FrameQueue.size() >= QueueCapacity) {
			H26XFrame *h26xFrame = H264FrameQueue.front();
			delete h26xFrame;
			H264FrameQueue.pop();
		}
		H26XFrame *h264Frame = new H26XFrame();

		h264Frame->FrameSize = FrameSize;
        h264Frame->Frame = (uint8_t*)malloc(h264Frame->FrameSize);
		memcpy(h264Frame->Frame, Frame, FrameSize);

		H264FrameQueue.push(h264Frame);
#endif

		pthread_mutex_unlock(&RWQueueMutex);

        h26xSource->doSignalNewFrameData();
	}

    return VVTK_RET_CALLBACK_CONTINUE;
}

H26XSource *
H26XSource::createNew(UsageEnvironment &env)
{
    return new H26XSource(env);
}

EventTriggerId H26XSource::eventTriggerId = 0;

unsigned H26XSource::referenceCount = 0;

H26XSource::H26XSource(UsageEnvironment &env) : FramedSource(env)
{
    if (referenceCount == 0)
    {
        vvtk_set_video_callback(1, videoSampleCapture, this);
        fPresentationTime.tv_sec = 0;
		fPresentationTime.tv_usec = 0;
    }
    ++referenceCount;

    if (eventTriggerId == 0)
    {
        eventTriggerId = envir().taskScheduler().createEventTrigger(deliverFrame0);
    }
}

H26XSource::~H26XSource()
{
    --referenceCount;
    if (referenceCount == 0)
    {
        pthread_mutex_destroy(&RWQueueMutex);

		while (!H264FrameQueue.empty())
		{
			H26XFrame *h26xFrame = H264FrameQueue.front();
			delete h26xFrame;
			H264FrameQueue.pop();
		}

        // Reclaim our 'event trigger'
        envir().taskScheduler().deleteEventTrigger(eventTriggerId);
        eventTriggerId = 0;
    }
}

void H26XSource::doGetNextFrame()
{
    while (H264FrameQueue.empty()) {
        usleep(5000);
    }
    deliverFrame();
}

void H26XSource::doStopGettingFrames() {

}

void H26XSource::deliverFrame0(void *clientData)
{
    ((H26XSource *)clientData)->deliverFrame();
}

void H26XSource::deliverFrame()
{
    if (!isCurrentlyAwaitingData()) {
        return; // we're not ready for the data yet
    }

#if 1
    pthread_mutex_lock(&RWQueueMutex);

    if (!H264FrameQueue.empty()) {
		H26XFrame *h26xFrame = H264FrameQueue.front();
		if (h26xFrame->FrameSize > fMaxSize) {
			fFrameSize = fMaxSize;
			fNumTruncatedBytes = h26xFrame->FrameSize - fMaxSize;
		}
		else {
			fFrameSize = h26xFrame->FrameSize;
		}

		fPresentationTime.tv_sec = h26xFrame->FramePTime.tv_sec;
		fPresentationTime.tv_usec = h26xFrame->FramePTime.tv_usec;

		memmove(fTo, h26xFrame->Frame, fFrameSize);
		delete h26xFrame;
		H264FrameQueue.pop();
	}
	else {
		fFrameSize = 0;
	}

    pthread_mutex_unlock(&RWQueueMutex);

    printf("H264 Frame Size: %d\t\tH264FrameQueueSize: %d\n", fFrameSize, H264FrameQueue.size());

    // After delivering the data, inform the reader that it is now available:
    if (fFrameSize > 0) {
    	FramedSource::afterGetting(this);
	}
#else
    pthread_mutex_lock(&RWQueueMutex);

    if (!H264FrameQueue.empty()) {
		H26XFrame *h26xFrame = H264FrameQueue.front();
        uint8_t *Frame = h26xFrame->Frame;
        uint32_t FrameSize = h26xFrame->FrameSize;
        uint32_t cursor = 0;

        /* Send NALU one by one */
        while (true) {
            auto nalu = H26X_ReadOneNaluFromBuffer(Frame, FrameSize, &cursor);
            if (nalu.empty()) {
                break;
            }
            if (nalu.size() > fMaxSize) {
                fFrameSize = fMaxSize;
                fNumTruncatedBytes = nalu.size() - fMaxSize;
            }
            else {
                fFrameSize = nalu.size();
            }
            fPresentationTime.tv_sec = h26xFrame->FramePTime.tv_sec;
		    fPresentationTime.tv_usec = h26xFrame->FramePTime.tv_usec;
            memmove(fTo, nalu.data(), fFrameSize);

            printf("H264 Frame Size: %d\t\tH264FrameQueueSize: %d\n", fFrameSize, H264FrameQueue.size());

            if (fFrameSize > 0) {
    	        FramedSource::afterGetting(this);
	        }
        }

        delete h26xFrame;
		H264FrameQueue.pop();		
	}

    pthread_mutex_unlock(&RWQueueMutex);
#endif
}

// The following code would be called to signal that a new frame of data has become available.
// This (unlike other "LIVE555 Streaming Media" library code) may be called from a separate thread.
// (Note, however, that "triggerEvent()" cannot be called with the same 'event trigger id' from different threads.
// Also, if you want to have multiple device threads, each one using a different 'event trigger id', then you will need
// to make "eventTriggerId" a non-static member variable of "H26XSource".)
void H26XSource::doSignalNewFrameData() {
    envir().taskScheduler().triggerEvent(eventTriggerId, this);
}

void signalNewFrameData()
{
    TaskScheduler *ourScheduler = NULL; //%%% TO BE WRITTEN %%%
    H26XSource *ourDevice = NULL;       //%%% TO BE WRITTEN %%%

    if (ourScheduler != NULL)
    { // sanity check
        ourScheduler->triggerEvent(H26XSource::eventTriggerId, ourDevice);
    }
}
