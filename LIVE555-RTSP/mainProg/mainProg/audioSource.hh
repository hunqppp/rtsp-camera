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

class AudioReceiver
{
public:
	static void doOpen();
	static void doClose();
	static size_t doReceive(unsigned char **p, uint32_t MaxSize);
public:
	static const char *UNIX_SOCKET_AUDIO_PATH;
	static int fd;
	static socklen_t addrClientLen;
	static struct sockaddr_un addrSocket;
	static struct sockaddr_un addrClient;
};

class AudioSource : public FramedSource
{

};

#endif /* __H26XSOURCE_HH */
