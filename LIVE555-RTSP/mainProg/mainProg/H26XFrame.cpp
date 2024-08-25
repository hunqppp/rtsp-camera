#include "H26XFrame.hh"

std::queue<H26XFrame*> H264FrameQueue;
pthread_mutex_t RWQueueMutex = PTHREAD_MUTEX_INITIALIZER;;

H26XFrame::H26XFrame() {
    Frame = NULL;
    FrameSize = 0;
    gettimeofday(&FramePTime, NULL);
}

H26XFrame::~H26XFrame() {
    if (Frame != NULL) {
        // delete Frame;
        free(Frame);
        Frame = NULL;
    }
}