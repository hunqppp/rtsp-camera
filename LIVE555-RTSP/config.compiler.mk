Print = @echo ""

#### Text bash color
KBLA='\033[0;30m'
KRED='\033[0;31m'
KGRN='\033[0;32m'
KYEL='\033[0;33m'
KBLU='\033[0;34m'
KPUR='\033[0;35m'
KCYC='\033[0;36m'
KWHI='\033[0;37m'
KRST='\e[0m'

CROSS_COMPILE		?=	$(HOME)/Toolchains/asdk-10.3.1-a7-EL-5.4-u1.0-a32nh-220218/bin/arm-linux-
COMPILE_OPTS 		=	$(INCLUDES) -I/usr/local/include -I. -O2 -DSOCKLEN_T=socklen_t -DNO_SSTREAM=1 -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64 -DNO_OPENSSL -DNEWLOCALE_NOT_USED=1 -DALLOW_RTSP_SERVER_PORT_REUSE=1

C 					=	c
C_COMPILER 			=	$(CROSS_COMPILE)gcc
C_FLAGS 			=	$(COMPILE_OPTS)
CPP 				=	cpp
CPLUSPLUS_COMPILER 	=	$(CROSS_COMPILE)g++
CPLUSPLUS_FLAGS 	=	$(COMPILE_OPTS) -Wall -DBSD=1
OBJ 				=	o
LINK 				=	$(CROSS_COMPILE)g++ -o
LINK_OPTS 			=		
CONSOLE_LINK_OPTS 	=	$(LINK_OPTS)
LIBRARY_LINK 		=	$(CROSS_COMPILE)ar cr 
LIBRARY_LINK_OPTS 	=	$(LINK_OPTS)
LIB_SUFFIX 			=	a
LIBS_FOR_CONSOLE_APPLICATION =
LIBS_FOR_GUI_APPLICATION =
EXE =

BUILD_DIR = build

# #### Compiler platform
# C =					c
# C_COMPILER =		cc
# CPP =				cpp
# CPLUSPLUS_COMPILER =c++

# ifeq ($(shell uname -m),x86_64)
# 	COMPILE_OPTS =		$(INCLUDES)	\
# 						-I/usr/local/include	\
# 						-I.	\
# 						-O2	\
# 						-DSOCKLEN_T=socklen_t	\
# 						-D_LARGEFILE_SOURCE=1	\
# 						-D_FILE_OFFSET_BITS=64	\
# 						-DALLOW_RTSP_SERVER_PORT_REUSE=1
	
# 	C_FLAGS =			$(COMPILE_OPTS)	\
# 						$(CPPFLAGS)	\
# 						$(CFLAGS)
	
# 	CPLUSPLUS_FLAGS =	$(COMPILE_OPTS)	\
# 						-Wall	\
# 						-DBSD=1	\
# 						$(CPPFLAGS)	\
# 						$(CXXFLAGS)

# 	OBJ =			o
# 	LINK =			c++ -o
# 	LINK_OPTS =		-L. $(LDFLAGS)
# 	CONSOLE_LINK_OPTS =	$(LINK_OPTS)
# 	LIBRARY_LINK =		ar cr 
# 	LIBRARY_LINK_OPTS =	
# 	LIB_SUFFIX =			a
# 	LIBS_FOR_CONSOLE_APPLICATION = -lssl -lcrypto
# 	LIBS_FOR_GUI_APPLICATION =
# 	EXE =

	
# else ifeq ($(shell uname -m),armv7l)

# else
# 	$(Print) $(KRED) Unknown compiler platform
# 	exit 1
# endif
