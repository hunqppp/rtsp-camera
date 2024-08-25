#ifndef __H26XFrame_H
#define __H26XFrame_H

#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include "GroupsockHelper.hh"

#define QueueCapacity   30 /* the H26XFrame Queue Capacity */

class H26XFrame {
public:
	uint8_t *Frame;
	uint32_t FrameSize;
	struct timeval FramePTime;

public:
	H26XFrame();
	~H26XFrame();
};

extern pthread_mutex_t RWQueueMutex;
extern std::queue<H26XFrame*> H264FrameQueue;

#endif /* __H26XFrame_H */