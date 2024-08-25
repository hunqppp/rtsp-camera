#ifndef __VVTK_AUDIO_H__
#define __VVTK_AUDIO_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "vvtk_def.h"

    typedef enum
    {
        VVTK_AUDIO_CODEC_AAC,
        VVTK_AUDIO_CODEC_G711_ALAW, /* G.711 A-law */
        VVTK_AUDIO_CODEC_G711_ULAW, /* G.711 μ-law */
        VVTK_AUDIO_CODEC_PCM,
        VVTK_AUDIO_CODEC_G726,
    } VVTK_AUDIO_CODEC;

    typedef enum
    {
        VVTK_AUDIO_SAMPLE_RATE_8_KHZ = 0,
        VVTK_AUDIO_SAMPLE_RATE_16_KHZ,
        VVTK_AUDIO_SAMPLE_RATE_32_KHZ,
        VVTK_AUDIO_SAMPLE_RATE_44_1_KHZ,
        VVTK_AUDIO_SAMPLE_RATE_48_KHZ,
    } VVTK_AUDIO_SAMPLE_RATE;

    typedef enum
    {
        VVTK_AUDIO_VOLUME_0 = 0, 
        VVTK_AUDIO_VOLUME_5,
        VVTK_AUDIO_VOLUME_10,
        VVTK_AUDIO_VOLUME_15,
        VVTK_AUDIO_VOLUME_20,
        VVTK_AUDIO_VOLUME_25,
        VVTK_AUDIO_VOLUME_30,
        VVTK_AUDIO_VOLUME_35,
        VVTK_AUDIO_VOLUME_40,
        VVTK_AUDIO_VOLUME_45,
        VVTK_AUDIO_VOLUME_50,
        VVTK_AUDIO_VOLUME_55,
        VVTK_AUDIO_VOLUME_60,
        VVTK_AUDIO_VOLUME_65,
        VVTK_AUDIO_VOLUME_70,
        VVTK_AUDIO_VOLUME_75,
        VVTK_AUDIO_VOLUME_80,
        VVTK_AUDIO_VOLUME_85,
        VVTK_AUDIO_VOLUME_90,
        VVTK_AUDIO_VOLUME_95,
        VVTK_AUDIO_VOLUME_100,
    } VVTK_AUDIO_VOLUME;

    typedef struct
    {
        VVTK_AUDIO_CODEC codec;
        VVTK_AUDIO_SAMPLE_RATE rate;
        int channel;
        int format;
    } vvtk_audio_config_t;

    typedef struct
    {
        VVTK_BYTE *data;
        int size;
        VVTK_AUDIO_CODEC codec;
        VVTK_AUDIO_SAMPLE_RATE sample_rate;
        uint64_t timestamp;
        int sequence;
        int index;
    } vvtk_audio_frame_t;

    typedef struct
    {
        int aecns_enable; /**> enable aecns, aec_enable and ns_enable can be set only when aecns_enable is 1 */
        int aec_en; /**> enable AEC  */
        int ns_en; /**> enable denoise */
        int aec_thr; /**> threshold for mute */
        int aec_scale; /**> coefficient of amplification for AEC, capture volume will be aec_scale*0.5x( value 0 is for 1x scale)*/
    } vvtk_audio_aec_t;

    /**
     * Callback function declation of the audio usage
     *
     * @param audio_frame Audio frame
     * @return VVTK_RET_CALLBACK stop or continue
     */
    typedef VVTK_RET_CALLBACK (*VVTK_AUDIO_CALLBACK)(vvtk_audio_frame_t *audio_frame, const void *user_data);

    /**
     * Callback function declation of the voice detect
     *
     * @return VVTK_RET_CALLBACK stop or continue
     */
    typedef VVTK_RET_CALLBACK (*VVTK_VOICE_DETECT_CALLBACK)(const void *user_data);

    typedef enum
    {
        VDOP_PID,           /**> Specified process will receivd SIGUSR1 if voice detected */
        VDOP_CALLBACK,      /**> Callback if voice detected */
    } VVTK_VOICE_DETECT_OP;

    typedef struct
    {
        int         voice_detect_coef;      /**> coefficient(0..10), default is 9 */
        uint64_t    voice_detect_thd;       /**> threshold, default is 0x1FFFFFFF */
        int         voice_detect_intv;      /**> event interval in seconds, default is 1 */
    } vvtk_voice_detect_t;

    typedef struct
    {
        VVTK_VOICE_DETECT_OP  vdop;
        union
        {
            int pid;                        /**> The process's pid to receive SIGUSR1 */
            struct
            {
                VVTK_VOICE_DETECT_CALLBACK cb;  /**> Callback function */
                void *user_data;                /**> User defined data */
            } callback;
        };
    } vvtk_vdop_t;

    /**
     * Immediately to get the related parameters used for capturing
     *
     * @see vvtk_audio_config_t
     *
     * @param index Audio Stream Index (0, 1, or 2)
     * @param config The settings used for capturing and encoding the video stream
     * @return VVTK_RET_SUCCESS for setting succeed, VVTK_RET_NOT_SUPPORTED presents the requested configuration is not supported for this device or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_get_audio_config(int index, vvtk_audio_config_t *config);

    /**
     * Immediately to set the related parameters used for capturing
     *
     * @see vvtk_audio_config_t
     *
     * NOTICE: PCM_16KHZ / 48KHZ / AAC_16KHZ / AAC_48KHZ should be supported
     * @param index Audio Stream Index (0, 1, or 2)
     * @param config The settings used for capturing and encoding the video stream
     * @return VVTK_RET_SUCCESS for setting succeed, VVTK_RET_NOT_SUPPORTED presents the requested configuration is not supported for this device or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_set_audio_config(int index, const vvtk_audio_config_t *config);

    /**
     * Immediately and continuously to get audio data captured from the microphone by Callback
     *
     * @see vvtk_audio_config_t
     *
     * @param index Audio Steam Index (0 or 1)
     * @param cb The callback function when capturing the audio stream
     * @return VVTK_RET_SUCCESS with audio to be captured, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_set_audio_callback(int index, const VVTK_AUDIO_CALLBACK cb, const void *user_data);

    /**
     * Initialize the CODEC when playing audio data in the function, vvtk_play_audio_output
     *
     * @param index The index of the audio output
     * @param config The settings of the audio output, currently only support VVTK_AUDIO_CODEC_PCM format
     * @return VVTK_RET_SUCCESS without errors, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_create_audio_output(int index, const vvtk_audio_config_t *config);

    /**
     * Immediately to release the internal resource
     *
     * @param index The index of the audio output
     * @return VVTK_RET_SUCCESS without errors, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_destory_audio_output(int index);

    /**
     * Get sound level for audio output (VVTK_AUDIO_VOLUME_0(Mute) to VVTK_AUDIO_VOLUME_100(Loudest))
     *
     * @param index The index of the audio output
     * @param volume The sound level (0(Mute) to VVTK_AUDIO_VOLUME_100(Loudest)) returned
     * @return VVTK_RET_SUCCESS with the volume value saved, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_get_audio_output_volume(VVTK_AUDIO_VOLUME *volume);

    /**
     * Set sound level for audio output (VVTK_AUDIO_VOLUME_0(Mute) to VVTK_AUDIO_VOLUME_100(Loudest))
     *
     * @param volume The sound level (VVTK_AUDIO_VOLUME_0(Mute) to VVTK_AUDIO_VOLUME_100(Loudest))
     * @return VVTK_RET_SUCCESS with the volume value saved, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_set_audio_output_volume(VVTK_AUDIO_VOLUME volume);

    /**
     * Immediately to play audio stream (Non-blocking)
     *
     * @param index The index of the audio output
     * @param buf The data of the audio stream
     * @param size The size of the audio data
     * @return VVTK_RET_SUCCESS without errors, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_play_audio(int index, const void *buf, int size);

    /**
     * Immediately to play the audio file supported by ALSA (Non-blocking)
     *
     * @param file_path the path of the audio file to be played
     * @return VVTK_RET_SUCCESS with the target Audio file played, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_play_audio_file(int index, const char *file_path);

    /**
     * Immediately to stop playing
     *
     * @return VVTK_RET_SUCCESS without errors, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_stop_audio(int index);

    /**
     * Get sound level for audio input (VVTK_AUDIO_VOLUME_0(Mute) to VVTK_AUDIO_VOLUME_100(Loudest))
     *
     * @param volume The sound level (VVTK_AUDIO_VOLUME_0(Mute) to VVTK_AUDIO_VOLUME_100(Loudest)) returned
     * @return VVTK_RET_SUCCESS with the volume value saved, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_get_audio_input_volume(VVTK_AUDIO_VOLUME *volume);

    /**
     * Set sound level for audio input (VVTK_AUDIO_VOLUME_0(Mute) to VVTK_AUDIO_VOLUME_100(Loudest))
     *
     * @param volume The sound level (VVTK_AUDIO_VOLUME_0(Mute) to VVTK_AUDIO_VOLUME_100(Loudest))
     * @return VVTK_RET_SUCCESS with the volume value saved, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_set_audio_input_volume(VVTK_AUDIO_VOLUME volume);

    /**
     * Immediately to get the related parameters for AEC.
     *
     * @see vvtk_audio_aec_t
     *
     * @param aec_config The settings used for aec
     * @return VVTK_RET_SUCCESS for setting succeed, VVTK_RET_NOT_SUPPORTED presents the requested configuration is not supported for this device or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_get_audio_aec_config(vvtk_audio_aec_t *aec_config);

    /**
     * Immediately to set the related parameters for AEC.
     *
     * The AEC will take effect when capture and playback samplerate is the same, and samplerate should be 8000Hz or 16000Hz.
     * @see vvtk_audio_aec_t
     *
     * @param aec_config The settings used for aec
     * @return VVTK_RET_SUCCESS for setting succeed, VVTK_RET_NOT_SUPPORTED presents the requested configuration is not supported for this device or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_set_audio_aec_config(const vvtk_audio_aec_t *aec_config);

    /**
     * Immediately to get the effectiveness of AEC.
     *
     * The AEC will take effect when capture and playback samplerate is the same, and samplerate should be 8000Hz or 16000Hz.
     * 
     * @param enable If true, AEC will take effect; if false, AEC will not take effect.
     * @return VVTK_RET_SUCCESS without errors, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_get_audio_aec_effectiveness(bool *enable);

    /**
     * Immediately to get the related parameters for voice detect.
     *
     * @see vvtk_voice_detect_t
     *
     * @param config The settings used for voice detect
     * @return VVTK_RET_SUCCESS without errors, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_get_voice_detect_config(vvtk_voice_detect_t *config);

    /**
     * Immediately to set the related parameters for voice detect.
     *
     * @see vvtk_voice_detect_t
     *
     * @param config The settings used for voice detect
     * @return VVTK_RET_SUCCESS for setting succeed, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_set_voice_detect_config(vvtk_voice_detect_t *config);

    /**
     * Immediately to start voice detect.
     *
     * @see vvtk_vdop_t
     *
     * There are two way to get voice detect signal.
     * One way is to create a process to capture SIGUSR1 signal. If voice detected will send SIGUSR1 to that process.
     * Another way is using callback function. If voice detected will inform by callback function.
     *
     * @param vd_op The operation for voice detect
     * @return VVTK_RET_SUCCESS without errors, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_start_voice_detect(vvtk_vdop_t *vd_op);

    /**
     * Immediately to stop voice detect.
     *
     * @return VVTK_RET_SUCCESS without errors, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_stop_voice_detect();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __VVTK_AUDIO_H__ */
