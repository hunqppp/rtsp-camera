#ifndef __ONVIF_DEFINITION_H
#define __ONVIF_DEFINITION_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Define wsd_simple_server.c 
*/
#define WSD_SIMPLE_SERVER_LOG_FILE              "/app/nvr/tmp/wsd_simple_server.log"
#define WSD_SIMPLE_SERVER_TEMPLATE_DIR          "/app/nvr/etc/wsd_simple_server"

/* Define onvif_simple_server.c 
*/
#define ONVIF_SIMPLE_SERVER_DEFAULT_CONF_FILE   "/app/nvr/etc/onvif_simple_server.conf"
#define ONVIF_SIMPLE_SERVER_DEFAULT_LOG_FILE    "/app/nvr/tmp/onvif_simple_server.log"
#define ONVIF_SIMPLE_SERVER_DEBUG_FILE          "/app/nvr/tmp/onvif_simple_server.debug"

/* Define onvif_notify_server.c 
*/
#define ONVIF_NOTIFY_SERVER_DEFAULT_CONF_FILE   "/app/nvr/etc/onvif_simple_server.conf"
#define ONVIF_NOTIFY_SERVER_TEMPLATE_DIR        "/app/nvr/etc/onvif_notify_server"
#define ONVIF_NOTIFY_SERVER_DEFAULT_LOG_FILE    "/app/nvr/tmp/onvif_notify_server.log"
#define ONVIF_NOTIFY_SERVER_DEFAULT_PID_FILE    "/app/nvr/tmp/onvif_notify_server.pid"
#define ONVIF_NOTIFY_SERVER_INOTIFY_DIR         "/app/nvr/tmp/onvif_notify_server"

#ifdef __cplusplus
}
#endif

#endif /* __ONVIF_DEFINITION_H */