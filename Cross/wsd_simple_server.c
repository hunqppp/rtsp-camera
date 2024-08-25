/*
 * Copyright (c) 2023 roleo.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include <getopt.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>

#include "utils.h"
#include "log.h"
#include "ezxml_wrapper.h"
#include "onvif_definition.h"
#include "debug.h"
#include "libvvtk/include/vvtk_system.h"

#define MULTICAST_ADDRESS       "239.255.255.250"
#define PORT                    3702
#define TYPE                    "NetworkVideoTransmitter"
     
#define DEFAULT_INTERFACE       "eth0"
#define DEFAULT_XADDRESS        "http://%s/onvif/device_service"   

#define DEFAULT_LOG_FILE        WSD_SIMPLE_SERVER_LOG_FILE
#define TEMPLATE_DIR            WSD_SIMPLE_SERVER_TEMPLATE_DIR

#define RECV_BUFFER_LEN         4096

#define BD_NO_CHDIR             01
#define BD_NO_CLOSE_FILES       02
#define BD_NO_REOPEN_STD_FDS    04

#define BD_NO_UMASK0            010
#define BD_MAX_CLOSE            8192

/* Static variables
*/
static const char *MODEL_NAME = (const char*)"IQ3S";
static const char *HARDWARE_MANUFACTURER = (const char*)"FPT_CAMERA";

static char sn[16];

static char *if_name = NULL;
static char *xaddr_s = NULL;

static int sock;
static int addr_len;
static struct ip_mreq mr;
static struct sockaddr_in addr_in;

static char template_file[1024];
static char address[16], netmask[16];

static char xaddr[1024];
static bool mainLoop;
static int msg_number;
static char uuid[UUID_LEN + 1];
static char msg_uuid[UUID_LEN + 1];

/* Static prototype functions 
*/
static void signal_handler(int signal);
static void print_usage(char *progname);
static void parser_options(int argc, char **argv);

static int socket_begin();
static int socket_send(const void *msg, size_t msgLen);
static int socket_receive(void *buffer, size_t bufferLen);

int main(int argc, char **argv)  {
    char tmp[32];
    long messageLen;
    int ret;
    char recv_buffer[RECV_BUFFER_LEN];
    char *message = NULL;

    parser_options(argc, argv);
    if (if_name == NULL) {
        if_name = DEFAULT_INTERFACE;
    }
    if (xaddr_s == NULL) {
        xaddr_s = DEFAULT_XADDRESS;
    }

    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    LOG_PRINT("Starting program.");
    LOG_PRINT("Argument if_name = %s", if_name);
    LOG_PRINT("Argument xaddr = %s", xaddr_s);

    /* Parse and get serial number
    */
    vvtk_system_set_rtsp_account("admin", "admin");
    memset(sn, 0, sizeof(sn));
    vvtk_system_get_sn(sn, sizeof(sn));
    MODEL_NAME = (const char*)sn;

    /* Check if TEMPLATE_DIR exists
    */
    bool boolean = false;
    if (access(TEMPLATE_DIR, F_OK ) != -1) {
        DIR *dir = NULL;
        if ((dir = opendir(TEMPLATE_DIR)) != NULL) {
            closedir (dir);
            boolean = true;
        }
    }
    if (!boolean) {
        LOG_ERROR("Unable to open directory %s", TEMPLATE_DIR);
        exit(EXIT_FAILURE);
    }

    /* Begin socket
    */
    ret = socket_begin();
    if (ret != 0) {
        exit(EXIT_FAILURE);
    }

    /* Prepare Hello message
    */
    msg_number = 1;
    sprintf(tmp, "%d", msg_number);
    gen_uuid(uuid);
    gen_uuid(msg_uuid);
    sprintf(xaddr, xaddr_s, address);

    /* Send Hello message
    */
    LOG_PRINT("Sending Hello message.");
    sprintf(template_file, "%s/Hello.xml", TEMPLATE_DIR);
    message = (char *)malloc((1024 * 2) * sizeof(char));
    if (message == NULL) {
        LOG_ERROR("Error malloc for Hello message.\n");
        shutdown(sock, SHUT_RDWR);
        exit(EXIT_FAILURE);
    }

    cat(message, template_file, 12,
        "%MSG_UUID%", msg_uuid,
        "%MSG_NUMBER%", tmp,
        "%UUID%", uuid,
        "%HARDWARE%", HARDWARE_MANUFACTURER,
        "%NAME%", MODEL_NAME,
        "%ADDRESS%", xaddr);

    addr_in.sin_addr.s_addr = inet_addr(MULTICAST_ADDRESS);
    ret = socket_send(message, strlen(message));
    free(message);
    if (ret == -1) {
        LOG_ERROR("Error sending Hello message.\n");
        shutdown(sock, SHUT_RDWR);
        exit(EXIT_FAILURE);
    }
    LOG_PRINT("Sent.");

    sleep(1);

    /*--------------------------------
     * Enter main loop
     *-------------------------------*/
    LOG_PRINT("Starting main loop");
    mainLoop = true;
    while (mainLoop) {
        /* Read from socket
        */
        memset(recv_buffer, '\0', RECV_BUFFER_LEN);
        if (socket_receive(recv_buffer, RECV_BUFFER_LEN) >= 0) {
            LOG_RECEIVED("RECIEVED BUFFER:");
            LOG_RECEIVED("%s", recv_buffer);

            /* Check if the message is a response
            */
            if ((strstr(recv_buffer, TYPE) != NULL) && (strstr(recv_buffer, "XAddrs") == NULL)) {
                const char *relates_to_uuid = NULL;

                /* Prepare ProbeMatches message
                */
                msg_number++;
                sprintf(tmp, "%d", msg_number);
                gen_uuid(msg_uuid);

                init_xml(recv_buffer, strlen(recv_buffer));
                relates_to_uuid = get_element("MessageID", "Header");
                if (relates_to_uuid == NULL) {
                    LOG_WARNING("Cannot find MessageID.\n");
                    continue;
                }
                close_xml();

                /* Send ProbeMatches message
                */
                LOG_PRINT("Sending ProbeMatches message.");
                sprintf(template_file, "%s/ProbeMatches.xml", TEMPLATE_DIR);
                messageLen = cat(NULL, template_file, 14,
                        "%MSG_UUID%", msg_uuid,
                        "%REL_TO_UUID%", relates_to_uuid,
                        "%MSG_NUMBER%", tmp,
                        "%UUID%", uuid,
                        "%HARDWARE%", HARDWARE_MANUFACTURER,
                        "%NAME%", MODEL_NAME,
                        "%ADDRESS%", xaddr);

                message = (char *) malloc((messageLen + 1) * sizeof(char));
                if (message == NULL) {
                    LOG_WARNING("Can't malloc message.\n");
                    continue;
                }

                cat(message, template_file, 14,
                        "%MSG_UUID%", msg_uuid,
                        "%REL_TO_UUID%", relates_to_uuid,
                        "%MSG_NUMBER%", tmp,
                        "%UUID%", uuid,
                        "%HARDWARE%", HARDWARE_MANUFACTURER,
                        "%NAME%", MODEL_NAME,
                        "%ADDRESS%", xaddr);

                LOG_SEND("SENT MESSAGE");
                LOG_SEND("%s", message);

                if (socket_send(message, strlen(message)) < 0) {
                    LOG_WARNING("Error sending ProbeMatches message.\n");
                }
                else {
                    LOG_PRINT("Sent.");
                }
                free(message);
            }
        }
    }

    shutdown(sock, SHUT_RDWR);

    LOG_PRINT("Terminating program.");

    return 0;
}

void print_usage(char *progname) {
    LOG_PRINT("\nUsage: %s -i INTERFACE -x XADDR\n\n", progname);
    LOG_PRINT("\t-i, --if_name\n");
    LOG_PRINT("\t\tNetwork interface\n");
    LOG_PRINT("\t-x, --xaddr\n");
    LOG_PRINT("\t\tResource address\n");
    LOG_PRINT("\t-h, --help\n");
    LOG_PRINT("\t\tPrint this help\n");
}

void signal_handler(int signal) {
    char tmp[32];
    int messageLen;
    char *message = NULL;

    /* Prepare and sed Bye message
    */
    msg_number++;
    sprintf(tmp, "%d", msg_number);
    gen_uuid(msg_uuid);

    LOG_PRINT("Sending Bye message.");
    sprintf(template_file, "%s/Bye.xml", TEMPLATE_DIR);
    messageLen = cat(NULL, template_file, 12,
            "%MSG_UUID%", msg_uuid,
            "%MSG_NUMBER%", tmp,
            "%UUID%", uuid,
            "%HARDWARE%", HARDWARE_MANUFACTURER,
            "%NAME%", MODEL_NAME,
            "%ADDRESS%", xaddr);

    message = (char *) malloc((messageLen + 1) * sizeof(char));
    if (message == NULL) {
        LOG_WARNING("Can't malloc Bye message.\n");
        shutdown(sock, SHUT_RDWR);
        mainLoop = false;
    }

    cat(message, template_file, 12,
            "%MSG_UUID%", msg_uuid,
            "%MSG_NUMBER%", tmp,
            "%UUID%", uuid,
            "%HARDWARE%", HARDWARE_MANUFACTURER,
            "%NAME%", MODEL_NAME,
            "%ADDRESS%", xaddr);

    if (socket_send(message, strlen(message)) < 0) {
        LOG_WARNING("Error sending Bye message.\n");
    }
    free(message);
    shutdown(sock, SHUT_RDWR);
    mainLoop = false;
}


void parser_options(int argc, char **argv) {
    const struct option long_options[] =
    {
        {"if_name"      ,  no_argument  , 0, 'i'},
        {"xaddr"        ,  no_argument  , 0, 'x'},
        {"help"         ,  no_argument  , 0, 'h'},
        {0              ,  0            , 0, 0  }
    };

    while (1) {
        int option_index = 0;
        int c = getopt_long (argc, argv, "i:x:h", long_options, &option_index);

        /* Detect the end of the options
        */
        if (c == -1) {
            break;
        }

        switch (c) {
        case 'i': {
            if_name = optarg;
        }
        break;

        case 'x': {
            xaddr_s = optarg;
        }
        break;

        case 'h': {
            print_usage(argv[0]);
            exit(EXIT_SUCCESS);
        }
        break;

        default: {
            print_usage(argv[0]);
            exit(EXIT_SUCCESS);
        }
        break;
        }
    }
}

int socket_begin() {
    bool boolean = 0;

    memset(&addr_in, '\0', sizeof(addr_in));
    addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(PORT);

    /* Waiting for IP address available
    */
    int ret = -1;
    while (ret == -1) { 
        ret = get_ip_address(address, netmask, if_name);
        if (ret != 0) {
            LOG_WARNING("Can't get IP network address");
            sleep(3);
        }
    }

    LOG_PRINT("Address = %s", address);
    LOG_PRINT("Netmask = %s", netmask);

    /* Open socket
    */
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) != -1) {
        if (bind(sock, (struct sockaddr *) &addr_in, sizeof(addr_in)) != -1) {
            boolean = true;
        }
        else {
            LOG_ERROR("Unable to bind socket\n");
        }
    }
    else {
        LOG_ERROR("Unable to create socket.\n");
    }

    if (boolean) {
        mr.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDRESS);
        mr.imr_interface.s_addr = inet_addr(address);
        if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &mr, sizeof(mr)) == -1) {
            LOG_ERROR("Error joining multicast group\n");
            boolean = false;
        }
    }

    if (!boolean) {
        shutdown(sock, SHUT_RDWR);
        return -1;
    }
    return 0;
}

int socket_send(const void *msg, size_t msgLen) {
    if (sock == -1) {
        return -1;
    }
    return sendto(sock, msg, msgLen, 0, (struct sockaddr *) &addr_in, sizeof(addr_in));
}

int socket_receive(void *buffer, size_t bufferLen) {
    if (sock == -1) {
        return -1;
    }
    return recvfrom(sock, buffer, bufferLen, 0, (struct sockaddr *) &addr_in, &addr_len);
}