# Compiler
CC 		= $(HOME)/Toolchains/asdk-10.3.1-a7-EL-5.4-u1.0-a32nh-220218/bin/arm-linux-gcc
CXX 	= $(HOME)/Toolchains/asdk-10.3.1-a7-EL-5.4-u1.0-a32nh-220218/bin/arm-linux-g++
STRIP 	= $(HOME)/Toolchains/asdk-10.3.1-a7-EL-5.4-u1.0-a32nh-220218/bin/arm-linux-strip

# Set HAVE_MBEDTLS variable if you want to use MBEDTLS instead of TOMCRYPT
HAVE_MBEDTLS=1

OBJECTS_O = onvif_simple_server.o device_service.o media_service.o ptz_service.o events_service.o fault.o conf.o utils.o log.o ezxml_wrapper.o ezxml/ezxml.o
OBJECTS_N = onvif_notify_server.o conf.o utils.o log.o ezxml_wrapper.o ezxml/ezxml.o
OBJECTS_W = wsd_simple_server.o utils.o log.o ezxml_wrapper.o ezxml/ezxml.o
ifdef HAVE_MBEDTLS
INCLUDE = -DHAVE_MBEDTLS -Iextras/mbedtls/include -ffunction-sections -fdata-sections -lrt
LIBS_O = -Wl,--gc-sections extras/mbedtls/library/libmbedcrypto.a -lpthread -lrt
LIBS_N = -Wl,--gc-sections extras/mbedtls/library/libmbedcrypto.a -lpthread -lrt
else
INCLUDE = -Iextras/libtomcrypt/src/headers -ffunction-sections -fdata-sections -lrt
LIBS_O = -Wl,--gc-sections extras/libtomcrypt/libtomcrypt.a -lpthread -lrt
LIBS_N = -Wl,--gc-sections extras/libtomcrypt/libtomcrypt.a -lpthread -lrt
endif
LIBS_W = -Wl,--gc-sections

# VIVOTEK Libraries extension
LDLIBS += -Llibvvtk/lib
LDLIBS += -lvvtkhal
CFLAGS += -I./libvvtk/include
CFLAGS += $(LDLIBS)

ifeq ($(STRIP), )
    STRIP=echo
endif

all: onvif_simple_server onvif_notify_server wsd_simple_server
	@cp wsd_simple_server  extras/_install/bin
	@cp onvif_simple_server  extras/_install/www/onvif
	@cd $(shell pwd)/extras/_install/www/onvif 	&&	\
	ln -s onvif_simple_server device_service 	&&	\
	ln -s onvif_simple_server events_service 	&&	\
	ln -s onvif_simple_server media_service		&&	\
	ln -s onvif_simple_server ptz_service
	@echo "--BUILD COMPLETED--"

# @sudo cp -r extras/_install $(HOME)/FTEL/nfs-point

log.o: log.c $(HEADERS)
	$(CC) -c $< -std=c99 -fPIC -Os $(INCLUDE) -o $@

%.o: %.c $(HEADERS)
	$(CC) -c $< -fPIC -Os $(INCLUDE) -o $@

onvif_simple_server: $(OBJECTS_O)
	$(CC) $(CFLAGS) $(OBJECTS_O) $(LIBS_O) -fPIC -Os -o $@
	$(STRIP) $@

onvif_notify_server: $(OBJECTS_N)
	$(CC) $(CFLAGS) $(OBJECTS_N) $(LIBS_N) -fPIC -Os -o $@
	$(STRIP) $@

wsd_simple_server: $(OBJECTS_W)
	$(CC) $(CFLAGS) $(OBJECTS_W) $(LIBS_W) -fPIC -Os -o $@
	$(STRIP) $@

run_lighttpd:
	@sudo $(shell pwd)/extras/_install/bin/lighttpd \
	-f $(shell pwd)/extras/_install/etc/lighttpd.conf \
	-m $(shell pwd)/extras/_install/lib -D
	@echo "Run lighttpd."

run_wsd_simple_server:
	@sudo $(shell pwd)/extras/_install/bin/wsd_simple_server \
	-i ens33 \
	-x http://%s:8080/onvif/device_service \
	-p /tmp/wsd_simple_server.pid \
	-d 5 \
	-f 1

.PHONY: clean

clean:
	rm -f onvif_simple_server
	rm -f onvif_notify_server
	rm -f wsd_simple_server
	rm -f $(OBJECTS_O)
	rm -f $(OBJECTS_N)
	rm -f $(OBJECTS_W)

#################################################################################################################################
# * ONVIF *
# /tmp/nfs/_install/bin/wsd_simple_server -i eth0 -x http://%s:8080/onvif/device_service -p /tmp/wsd_simple_server.pid -d 5 -f 1
# * HTTP *
# /tmp/nfs/_install/bin/lighttpd -f /tmp/nfs/_install/etc/lighttpd.conf -D -m /tmp/nfs/_install/lib
#################################################################################################################################