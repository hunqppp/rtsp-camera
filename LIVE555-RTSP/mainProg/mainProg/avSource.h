#ifndef __AV_SOURCE_H
#define __AV_SOURCE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#include "vvtk/include/vvtk_video.h"
#include "vvtk/include/vvtk_audio.h"
#include "vvtk/include/vvtk_system.h"

#define UNIX_SOCKET_VIDEO   (const char*)"/tmp/video.sock"
#define UNIX_SOCKET_AUDIO   (const char*)"/tmp/audio.sock"

enum {
    AVSOCKET_RET_FAILURE = 0,
    AVSOCKET_RET_SUCCESS,
};

extern void avSourceBegin();
extern int8_t avSourceOpenSocketSend();
extern void avSourceCloseSocketSend();

#ifdef __cplusplus
}
#endif

#endif /* __AV_SOURCE_H */