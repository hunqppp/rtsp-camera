#include "avSource.h"

static const bool bSendAudio = false;
static const bool bSendVideo = true;

static const uint8_t VIDEO_CHANNEL = 1;
static const uint8_t AUDIO_CHANNEL = 0;

static int fdSocketVideo = -1;
static struct sockaddr_un addressVideoProfile;
static int fdSocketAudio = -1;
static struct sockaddr_un addressAudioProfile;

static VVTK_RET_CALLBACK audioSampleCapture(vvtk_audio_frame_t *audioFrame, const void *arg);
static VVTK_RET_CALLBACK videoSampleCapture(const vvtk_video_frame_t *videoFrame, const void *arg);

void avSourceBegin() {
	vvtk_system_set_rtsp_account("hunqp", "1234");
	return;

	vvtk_video_config_t config;

	/*
		VGA		640  x 360
		720P	1280 x 720
		1080P	1920 x 1080
		2K		2304 x 1296
	*/

	/* Channel 3MP */
	{
		vvtk_get_video_config(0, &config);
		config.width 			= 1920;
		config.height 			= 1080;
		config.frame_rate 		= 15;
		config.gop				= 30;
		config.encoding_mode	= VVTK_VIDEO_ENCODING_MODE_VBR;
		if (config.encoding_mode == VVTK_VIDEO_ENCODING_MODE_VBR) {
			config.bitrate_max = 1024 * 2048;
			config.bitrate_min = 1024 * 2048;
			// config.bitrate_max = 2048;
			// config.bitrate_min = 2048;
		}
		else if (config.encoding_mode == VVTK_VIDEO_ENCODING_MODE_CBR) {
			config.bitrate_max = 1024 * 1024;
		}
		config.vbr_quality 		= VVTK_VBR_QUALITY_BETTER;
		config.codec 			= VVTK_VIDEO_CODEC_H264;
		vvtk_set_video_config(0, &config);
	}

	/* Channel HD */
	{
		vvtk_get_video_config(1, &config);
		config.width 			= 640;
		config.height 			= 360;
		config.frame_rate 		= 15;
		config.gop				= 30;
		config.encoding_mode	= VVTK_VIDEO_ENCODING_MODE_VBR;
		if (config.encoding_mode == VVTK_VIDEO_ENCODING_MODE_VBR) {
			config.bitrate_max = 256 * 1024;
			config.bitrate_min = 256 * 1024;
		}
		else if (config.encoding_mode == VVTK_VIDEO_ENCODING_MODE_CBR) {
			config.bitrate_max = 1024 * 1024;
		}
		config.vbr_quality 		= VVTK_VBR_QUALITY_GOOD;
		config.codec 			= VVTK_VIDEO_CODEC_H264;
		vvtk_set_video_config(1, &config);
	}
}

int8_t avSourceOpenSocketSend() {
    if (bSendVideo) {
		fdSocketVideo = socket(AF_UNIX, SOCK_DGRAM, 0);
		if (fdSocketVideo == -1) {
			return AVSOCKET_RET_FAILURE;
		}
		memset(&addressVideoProfile, 0, sizeof(addressVideoProfile));
		addressVideoProfile.sun_family = AF_UNIX;
		strncpy(addressVideoProfile.sun_path, UNIX_SOCKET_VIDEO, sizeof(addressVideoProfile.sun_path) - 1);		 

        vvtk_set_video_callback(VIDEO_CHANNEL, videoSampleCapture, NULL); /* 1 is H264 HD Channel */
	}
	
	if (bSendAudio) {
		fdSocketAudio = socket(AF_UNIX, SOCK_DGRAM, 0);
		if (fdSocketAudio == -1) {
			return AVSOCKET_RET_FAILURE;
		}
		memset(&addressAudioProfile, 0, sizeof(addressAudioProfile));
		addressAudioProfile.sun_family = AF_UNIX;
		strncpy(addressAudioProfile.sun_path, UNIX_SOCKET_AUDIO, sizeof(addressAudioProfile.sun_path) - 1);

        vvtk_set_audio_callback(AUDIO_CHANNEL, audioSampleCapture, NULL); /* 0 is ALaw Channel */
	}

    return AVSOCKET_RET_SUCCESS;
}

void avSourceCloseSocketSend() {
    if (fdSocketVideo != -1) {
        close(fdSocketVideo);
    }

    if (fdSocketAudio != -1) {
        close(fdSocketAudio);
    }
}

VVTK_RET_CALLBACK audioSampleCapture(vvtk_audio_frame_t *audioFrame, const void *arg) {
    (void)arg;
    if (audioFrame->size) {
		if (fdSocketAudio != -1) {
			sendto(fdSocketAudio, audioFrame->data, audioFrame->size, 0, (struct sockaddr*)&addressAudioProfile, sizeof(addressAudioProfile));
		}
	}

    return VVTK_RET_CALLBACK_CONTINUE;
}

VVTK_RET_CALLBACK videoSampleCapture(const vvtk_video_frame_t *videoFrame, const void *arg) {
    (void)arg;
	if (videoFrame->size) {
		if (fdSocketVideo != -1) {
			sendto(fdSocketVideo, videoFrame->data, videoFrame->size, 0, (struct sockaddr*)&addressVideoProfile, sizeof(addressVideoProfile));
		}
	}

    return VVTK_RET_CALLBACK_CONTINUE;
}