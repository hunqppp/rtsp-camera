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

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <limits.h>
#include <libgen.h>

#include "onvif_simple_server.h"
#include "device_service.h"
#include "media_service.h"
#include "ptz_service.h"
#include "events_service.h"
#include "fault.h"
#include "ezxml_wrapper.h"
#include "conf.h"
#include "utils.h"
#include "log.h"
#include "onvif_definition.h"
#include "debug.h"

#define DEFAULT_CONF_FILE   ONVIF_SIMPLE_SERVER_DEFAULT_CONF_FILE
#define DEFAULT_LOG_FILE    ONVIF_SIMPLE_SERVER_DEFAULT_LOG_FILE
#define DEBUG_FILE          ONVIF_SIMPLE_SERVER_DEBUG_FILE

service_context_t service_ctx;

int main(int argc, char ** argv) {
    char *tmp;
    int ret;
    char conf_file[strlen(DEFAULT_CONF_FILE) +1];
    char *prog_name;
    const char *method;
    username_token_t security;
    int auth_error = 0;

    /* Check file configure existence */
    memset(conf_file, 0, sizeof(conf_file));
    strcpy(conf_file, DEFAULT_CONF_FILE);
    if (access(conf_file, F_OK) != 0)  {
        exit(EXIT_FAILURE);
    }

    /* Check if the service name is sent as a last argument
    */
    if (argc > 1) {
        tmp = argv[argc - 1];
        if ((strstr(tmp, "device_service") != NULL) ||  (strstr(tmp, "media_service") != NULL) ||
            (strstr(tmp, "ptz_service") != NULL) || (strstr(tmp, "events_service") != NULL))
        {
            tmp = argv[argc - 1];
        }
        else {
            tmp = argv[0];
        }
    }
    else {
        tmp = argv[0];
    }
    prog_name = basename(tmp);

    ret = process_conf_file(conf_file);
    if (ret < 0) {
        exit(EXIT_FAILURE);
    }
    
    tmp = getenv("REQUEST_METHOD");
    if ((tmp == NULL) || (strcmp("POST", tmp) != 0)) {
        fprintf(stdout, "Content-type: text/html\r\n");
        fprintf(stdout, "Content-Length: 86\r\n\r\n");
        fprintf(stdout, "<html><head><title>Error</title></head><body>HTTP method not supported</body></html>\r\n");
        exit(EXIT_FAILURE);
    }

    int input_size;
    char *input = (char *)malloc (16 * 1024 * sizeof(char));
    if (input == NULL) {
        exit(EXIT_FAILURE);
    }
    input_size = fread(input, 1, 16 * 1024 * sizeof(char), stdin);
    if (input_size == 0) {
        free(input);
        exit(EXIT_FAILURE);
    }
    input = (char *)realloc(input, input_size * sizeof(char));
    /*  Realloc returns new address which is not always the same as "input", 
        if not updated the old pointer address causes segmentation fault when trying to use teh variable
    */
    if (input == NULL) {
        free(input);
        exit(EXIT_FAILURE);
    }
    
    /* Warning: init_xml changes the input string
    */
    init_xml(input, input_size);
    method = get_method(1);
    if (method == NULL) {
        close_xml();
        free(input);
        exit(EXIT_FAILURE);
    }

    if (service_ctx.user != NULL) {
        if ((get_element("Security", "Header") != NULL) && (get_element("UsernameToken", "Header") != NULL)) {
            unsigned long nonce_size = 128;
            unsigned long auth_size = 128;
            unsigned long sha1_size = 20;
            unsigned long digest_size = 128;
            unsigned char nonce[128];
            unsigned char auth[128];
            char sha1[20];
            char digest[128];

            security.enable = 1;
            security.username = get_element("Username", "Header");
            if (security.username != NULL) {
                log_debug("Security: username = %s", security.username);
            } 
            else {
                auth_error = 1;
            }
            security.password = get_element("Password", "Header");
            if (security.password != NULL) {
                log_debug("Security: password = ********");
            } 
            else {
                auth_error = 2;
            }
            security.nonce = get_element("Nonce", "Header");
            if (security.nonce != NULL) {
                log_debug("Security: nonce = %s", security.nonce);
                b64_decode((unsigned char *) security.nonce, strlen(security.nonce), nonce, &nonce_size);
            } 
            else {
                auth_error = 3;
            }
            security.created = get_element("Created", "Header");
            if (security.created != NULL) {
                log_debug("Security: created = %s", security.created);
            } 
            else {
                auth_error = 4;
            }

            if (auth_error == 0) {
                // Calculate digest and check the password
                // Digest = B64ENCODE( SHA1( B64DECODE( Nonce ) + Date + Password ) )
                memcpy(auth, nonce, nonce_size);
                memcpy(&auth[nonce_size], security.created, strlen(security.created));
                memcpy(&auth[nonce_size + strlen(security.created)], service_ctx.password, strlen(service_ctx.password));
                auth_size = nonce_size + strlen(security.created) + strlen(service_ctx.password);
                hashSHA1(auth, auth_size, sha1, sha1_size);
                b64_encode(sha1, sha1_size, digest, &digest_size);
                log_debug("Calculated digest: %s", digest);
                log_debug("Received digest: %s", security.password);

                if ((strcmp(service_ctx.user, security.username) != 0) ||
                        (strcmp(security.password, digest) != 0)) {

                    auth_error = 10;
                }
            }
        } 
        else {
            auth_error = 11;
        }
    } 
    else {
        security.enable = 0;
    }

    if (security.enable == 1) {
        if (auth_error == 0)
            log_info("Authentication ok");
        else
            log_error("Authentication error");
    }

    if ((strcmp("GetSystemDateAndTime", method) == 0) || (strcmp("GetUsers", method) == 0)) {
        auth_error = 0;
    }

    if (auth_error == 0) {

        if (strcasecmp("device_service", prog_name) == 0) {
            if (strcasecmp(method, "GetServices") == 0) {
                device_get_services();
            } 
            else if (strcasecmp(method, "GetServiceCapabilities") == 0) {
                device_get_service_capabilities();
            } 
            else if (strcasecmp(method, "GetDeviceInformation") == 0) {
                device_get_device_information();
            } 
            else if (strcasecmp(method, "GetSystemDateAndTime") == 0) {
                device_get_system_date_and_time();
            } 
            else if (strcasecmp(method, "SystemReboot") == 0) {
                device_system_reboot();
            } 
            else if (strcasecmp(method, "GetScopes") == 0) {
                device_get_scopes();
            } 
            else if (strcasecmp(method, "GetUsers") == 0) {
                device_get_users();
            } 
            else if (strcasecmp(method, "GetWsdlUrl") == 0) {
                device_get_wsdl_url();
            } 
            else if (strcasecmp(method, "GetCapabilities") == 0) {
                device_get_capabilities();
            } 
            else if (strcasecmp(method, "GetNetworkInterfaces") == 0) {
                device_get_network_interfaces();
            } 
            else {
                device_unsupported(method);
            }
        } else if (strcasecmp("media_service", prog_name) == 0) {
            if (strcasecmp(method, "GetServiceCapabilities") == 0) {
                media_get_service_capabilities();
            } 
            else if (strcasecmp(method, "GetVideoSources") == 0) {
                media_get_video_sources();
            } 
            else if (strcasecmp(method, "GetVideoSourceConfigurations") == 0) {
                media_get_video_source_configurations();
            } 
            else if (strcasecmp(method, "GetVideoSourceConfiguration") == 0) {
                media_get_video_source_configuration();
            } 
            else if (strcasecmp(method, "GetCompatibleVideoSourceConfigurations") == 0) {
                media_get_compatible_video_source_configurations();
            } 
            else if (strcasecmp(method, "GetVideoSourceConfigurationOptions") == 0) {
                media_get_video_source_configuration_options();
            } 
            else if (strcasecmp(method, "GetProfiles") == 0) {
                media_get_profiles();
            } 
            else if (strcasecmp(method, "GetProfile") == 0) {
                media_get_profile();
            } 
            else if (strcasecmp(method, "CreateProfile") == 0) {
                media_create_profile();
            } 
            else if (strcasecmp(method, "GetVideoEncoderConfigurations") == 0) {
                media_get_video_encoder_configurations();
            } 
            else if (strcasecmp(method, "GetVideoEncoderConfiguration") == 0) {
                media_get_video_encoder_configuration();
            } 
            else if (strcasecmp(method, "GetCompatibleVideoEncoderConfigurations") == 0) {
                media_get_compatible_video_encoder_configurations();
            } 
            else if (strcasecmp(method, "GetVideoEncoderConfigurationOptions") == 0) {
                media_get_video_encoder_configuration_options();
            } 
            else if (strcasecmp(method, "GetGuaranteedNumberOfVideoEncoderInstances") == 0) {
                media_get_guaranteed_number_of_video_encoder_instances();
            } 
            else if (strcasecmp(method, "GetSnapshotUri") == 0) {
                media_get_snapshot_uri();
            } 
            else if (strcasecmp(method, "GetStreamUri") == 0) {
                media_get_stream_uri();
            } 
            else if (strcasecmp(method, "GetAudioSources") == 0) {
                media_get_audio_sources();
            } 
            else if (strcasecmp(method, "GetAudioSourceConfigurations") == 0) {
                media_get_audio_source_configurations();
            } 
            else if (strcasecmp(method, "GetAudioSourceConfiguration") == 0) {
                media_get_audio_source_configuration();
            } 
            else if (strcasecmp(method, "GetAudioSourceConfigurationOptions") == 0) {
                media_get_audio_source_configuration_options();
            } 
            else if (strcasecmp(method, "GetAudioEncoderConfigurations") == 0) {
                media_get_audio_encoder_configurations();
            } 
            else if (strcasecmp(method, "GetAudioEncoderConfiguration") == 0) {
                media_get_audio_encoder_configuration();
            } 
            else if (strcasecmp(method, "GetAudioEncoderConfigurationOptions") == 0) {
                media_get_audio_encoder_configuration_options();
            } 
            else if (strcasecmp(method, "GetAudioDecoderConfigurations") == 0) {
                media_get_audio_decoder_configurations();
            } 
            else if (strcasecmp(method, "GetAudioDecoderConfiguration") == 0) {
                media_get_audio_decoder_configuration();
            } 
            else if (strcasecmp(method, "GetAudioDecoderConfigurationOptions") == 0) {
                media_get_audio_decoder_configuration_options();
            } 
            else if (strcasecmp(method, "GetAudioOutputs") == 0) {
                media_get_audio_outputs();
            } 
            else if (strcasecmp(method, "GetAudioOutputConfiguration") == 0) {
                media_get_audio_output_configuration();
            } 
            else if (strcasecmp(method, "GetAudioOutputConfigurations") == 0) {
                media_get_audio_output_configurations();
            } 
            else if (strcasecmp(method, "GetAudioOutputConfigurationOptions") == 0) {
                media_get_audio_output_configuration_options();
            } 
            else if (strcasecmp(method, "GetCompatibleAudioSourceConfigurations") == 0) {
                media_get_compatible_audio_source_configurations();
            } 
            else if (strcasecmp(method, "GetCompatibleAudioEncoderConfigurations") == 0) {
                media_get_compatible_audio_encoder_configurations();
            } 
            else if (strcasecmp(method, "GetCompatibleAudioDecoderConfigurations") == 0) {
                media_get_compatible_audio_decoder_configurations();
            } 
            else if (strcasecmp(method, "GetCompatibleAudioOutputConfigurations") == 0) {
                media_get_compatible_audio_output_configurations();
            } 
            else {
                media_unsupported(method);
            }
        } 
        else if (strcasecmp("ptz_service", prog_name) == 0) {
            if (strcasecmp(method, "GetServiceCapabilities") == 0) {
                ptz_get_service_capabilities();
            } 
            else if (strcasecmp(method, "GetConfigurations") == 0) {
                ptz_get_configurations();
            } 
            else if (strcasecmp(method, "GetConfiguration") == 0) {
                ptz_get_configuration();
            } 
            else if (strcasecmp(method, "GetConfigurationOptions") == 0) {
                ptz_get_configuration_options();
            } 
            else if (strcasecmp(method, "GetNodes") == 0) {
                ptz_get_nodes();
            } 
            else if (strcasecmp(method, "GetNode") == 0) {
                ptz_get_node();
            } 
            else if (strcasecmp(method, "GetPresets") == 0) {
                ptz_get_presets();
            } 
            else if (strcasecmp(method, "GotoPreset") == 0) {
                ptz_goto_preset();
            } 
            else if (strcasecmp(method, "GotoHomePosition") == 0) {
                ptz_goto_home_position();
            } 
            else if (strcasecmp(method, "ContinuousMove") == 0) {
                ptz_continuous_move();
            } 
            else if (strcasecmp(method, "RelativeMove") == 0) {
                ptz_relative_move();
            } 
            else if (strcasecmp(method, "AbsoluteMove") == 0) {
                ptz_absolute_move();
            } 
            else if (strcasecmp(method, "Stop") == 0) {
                ptz_stop();
            } 
            else if (strcasecmp(method, "GetStatus") == 0) {
                ptz_get_status();
            } 
            else if (strcasecmp(method, "SetPreset") == 0) {
                ptz_set_preset();
            } 
            else if (strcasecmp(method, "SetHomePosition") == 0) {
                ptz_set_home_position();
            } 
            else if (strcasecmp(method, "RemovePreset") == 0) {
                ptz_remove_preset();
            } 
            else {
                ptz_unsupported(method);
            }
        } 
        else if (strcasecmp("events_service", prog_name) == 0) {
            if (strcasecmp(method, "GetServiceCapabilities") == 0) {
                events_get_service_capabilities();
            } 
            else if (strcasecmp(method, "CreatePullPointSubscription") == 0) {
                events_create_pull_point_subscription();
            } 
            else if (strcasecmp(method, "PullMessages") == 0) {
                events_pull_messages();
            } 
            else if (strcasecmp(method, "Subscribe") == 0) {
                events_subscribe();
            } 
            else if (strcasecmp(method, "Renew") == 0) {
                events_renew();
            } 
            else if (strcasecmp(method, "Unsubscribe") == 0) {
                events_unsubscribe();
            } 
            else if (strcasecmp(method, "GetEventProperties") == 0) {
                events_get_event_properties();
            } 
            else if (strcasecmp(method, "SetSynchronizationPoint") == 0) {
                events_set_synchronization_point();
            } 
            else {
                events_unsupported(method);
            }
        }
    } else {
        // hack to handle a bug with Synology
        if ((service_ctx.adv_synology_nvr == 1) && 
                (strcasecmp("media_service", prog_name) == 0) &&
                (strcasecmp("CreateProfile", method) == 0)) {

            send_fault("media_service", "Receiver", "ter:Action", "ter:MaxNVTProfiles", "Max profile number reached", "The maximum number of supported profiles supported by the device has been reached");
        } 
        else {
            send_authentication_error();
        }
    }

    close_xml();
    free(input);
    free_conf_file();

    return 0;
}
