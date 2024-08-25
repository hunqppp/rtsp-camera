#ifndef __VVTK_VIDEO_H__
#define __VVTK_VIDEO_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "vvtk_def.h"

    typedef enum
    {
        VVTK_VIDEO_CODEC_H264 = 0,
        VVTK_VIDEO_CODEC_H265,
        VVTK_VIDEO_CODEC_MJPEG,
    } VVTK_VIDEO_CODEC;

    typedef enum
    {
        VVTK_RAW_IMAGE_FORMAT_YUV_420_888 = 0,
        VVTK_RAW_IMAGE_FORMAT_YUV_422_888,
        VVTK_RAW_IMAGE_FORMAT_YUV_444_888,
        VVTK_RAW_IMAGE_FORMAT_RGB_565,
        VVTK_RAW_IMAGE_FORMAT_BGR_565,
        VVTK_RAW_IMAGE_FORMAT_RGB_888,
        VVTK_RAW_IMAGE_FORMAT_BGR_888,
        VVTK_RAW_IMAGE_FORMAT_ARGB_8888,
        VVTK_RAW_IMAGE_FORMAT_ABGR_8888,
        VVTK_RAW_IMAGE_FORMAT_RGBA_8888,
        VVTK_RAW_IMAGE_FORMAT_BGRA_8888,
    } VVTK_RAW_IMAGE_FORMAT;

    typedef enum
    {
        VVTK_VBR_QUALITY_VERY_HIGH = 22,
        VVTK_VBR_QUALITY_HIGH = 26,
        VVTK_VBR_QUALITY_EXCELLENT = 30,
        VVTK_VBR_QUALITY_BETTER = 32,
        VVTK_VBR_QUALITY_VERY_GOOD = 34,
        VVTK_VBR_QUALITY_GOOD = 36,
        VVTK_VBR_QUALITY_NORMAL = 38,
        VVTK_VBR_QUALITY_LOW = 40,
        VVTK_VBR_QUALITY_LOWER = 42
    } VVTK_VBR_QUALITY;

    typedef enum
    {
        VVTK_VIDEO_ENCODING_MODE_CBR = 0,
        VVTK_VIDEO_ENCODING_MODE_CVBR,
        VVTK_VIDEO_ENCODING_MODE_VBR,
    } VVTK_VIDEO_ENCODING_MODE;

    typedef enum
    {
        VVTK_VIDEO_FRAME_TYPE_IDR = 0,
        VVTK_VIDEO_FRAME_TYPE_I,
        VVTK_VIDEO_FRAME_TYPE_P,
        VVTK_VIDEO_FRAME_TYPE_B,
    } VVTK_VIDEO_FRAME_TYPE;

    typedef struct
    {
        VVTK_VIDEO_CODEC codec;
        int width;                              /**> The width of a video frame, in pixels */
        int height;                             /**> The height of a video frame, in pixels */
        int frame_rate;                         /**> The frame rate to be captured. (1-30 fps, frames per second) */
        VVTK_VIDEO_ENCODING_MODE encoding_mode; /**> CBR/CVBR/VBR */
        int gop;                                /**> Group of pictures of the stream. (1-30) */
        int bitrate_min;                        /**> The minimum bitrate (bps), used in CVBR. */
        int bitrate_max;                        /**> The maximum bitrate (bps), used in CBR/CVBR. CBR will reference bitrate_max only */
        VVTK_VBR_QUALITY vbr_quality;           /**> The VBR quality, used in VBR */
    } vvtk_video_config_t;

    typedef struct
    {
        VVTK_RAW_IMAGE_FORMAT image_format; /**> RAW image format */
        int width;                          /**> The width of a video frame, in pixels */
        int height;                         /**> The height of a video frame, in pixels */
        int frame_rate;                     /**> The frame rate to be captured. (1-30 fps, frames per second) */
    } vvtk_raw_video_config_t;

    typedef struct
    {
        VVTK_BYTE *data;
        int size;
        int width;
        int height;
        VVTK_VIDEO_FRAME_TYPE type;
        uint64_t timestamp; /**> Unix epoch time in milliseconds */
        int sequence;       /**> Frame sequence number. Start at 0. Reset to 0 when equal to INT_MAX. */
        int index;
    } vvtk_video_frame_t;

    typedef struct
    {
        VVTK_BYTE *data;
        int size;
        int width;
        int height;
        uint64_t timestamp; /**> Unix epoch time in milliseconds */
        int sequence;       /**> Frame sequence number. Start at 0. Reset to 0 when equal to INT_MAX. */
        int index;
    } vvtk_raw_video_frame_t;

    typedef struct
    {
        int width;
        int height;
        int quality;
    } vvtk_jpeg_config_t;

    typedef struct
    {
        VVTK_BYTE *data;
        int size;
    } vvtk_jpeg_image_t;

    /**
     * Callback function declation of the video stream usage
     *
     * @see vvtk_video_frame_t
     *
     * @param video_frame The video frame data
     * @return VVTK_RET_CALLBACK stop or continue
     */
    typedef VVTK_RET_CALLBACK (*VVTK_VIDEO_CALLBACK)(const vvtk_video_frame_t *video_frame, const void *user_data);

    /**
     * Callback function declation of the raw video stream usage
     *
     * @see vvtk_raw_video_frame_t
     *
     * @param video_frame The raw video frame data
     * @return VVTK_RET_CALLBACK stop or continue
     */
    typedef VVTK_RET_CALLBACK (*VVTK_RAW_VIDEO_CALLBACK)(const vvtk_raw_video_frame_t *video_frame, const void *user_data);

    /**
     * Immediately to get the related parameters used for capturing
     *
     * @see vvtk_video_config_t
     *
     * @param index Video Stream Index (0, 1, 2 or 3)
     * @param config The settings used for capturing and encoding the video stream
     * @return VVTK_RET_SUCCESS for setting succeed, VVTK_RET_NOT_SUPPORTED presents the requested configuration is not supported for this device or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_get_video_config(int index, vvtk_video_config_t *config);

    /**
     * Immediately to set the related parameters used for capturing
     *
     * @see vvtk_video_config_t
     *
     * @param index Video Stream Index (0, 1, 2 or 3)
     * @param config The settings used for capturing and encoding the video stream. If the encoding_mode is VBR, vbr_quality will effect after config.
     *               But after reboot, vbr_quality will restore to VVTK_VBR_QUALITY_EXCELLENT, you need reconfig to another value. 
     * @return VVTK_RET_SUCCESS for setting succeed, VVTK_RET_NOT_SUPPORTED presents the requested configuration is not supported for this device or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_set_video_config(int index, const vvtk_video_config_t *config);

    /**
     * Immediately and continuously to capture from the camera and encode to H.264 by Callback
     *
     * @param index Video Stream Index (0, 1, 2 or 3)
     * @param cb The callback function when capturing the video stream
     * @return VVTK_RET_SUCCESS for setting succeed, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_set_video_callback(int index, VVTK_VIDEO_CALLBACK cb, const void *user_data);

    /**
     * Immediately to set the related parameters used for capturing
     *
     * @see vvtk_raw_video_config_t
     *
     * @param index Video stream index (0, 1, 2 or 3)
     * @param config The settings used for capturing and encoding the video stream
     * @return VVTK_RET_SUCCESS for setting succeed, VVTK_RET_NOT_SUPPORTED presents the requested configuration is not supported for this device or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_set_raw_video_config(int index,const vvtk_raw_video_config_t *config);

    /**
     * Immediately and continuously to capture from the camera by callback
     *
     * @param index Video stream index (0, 1, 2 or 3)
     * @param cb The callback function when capturing the video stream
     * @return VVTK_RET_SUCCESS for setting succeed, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_set_raw_video_callback(int index, VVTK_RAW_VIDEO_CALLBACK cb, const void *user_data);

    /**
     * Immediately to capture and save one jpeg frame from the camera sensor (Non-blocking)
     *
     * @param config The settings used for getting a image
     * @param image The jpeg image get from the camera sensor
     * @return VVTK_RET_SUCCESS with the target jpeg image saved, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_get_jpeg_image(const vvtk_jpeg_config_t *config,vvtk_jpeg_image_t *image);

    /**
     * Immediately free the captured jpeg image
     *
     * @param image The jpeg image get from the camera sensor
     * @return VVTK_RET_SUCCESS with the target jpeg image saved, or other VVTK_RET error(s)
     */
    VVTK_RET vvtk_free_jpeg_image(vvtk_jpeg_image_t *image);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __VVTK_VIDEO_H__ */