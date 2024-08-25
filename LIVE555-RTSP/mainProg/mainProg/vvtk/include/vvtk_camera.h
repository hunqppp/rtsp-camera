#ifndef __VVTK_CAMERA_H__
#define __VVTK_CAMERA_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <vvtk_def.h>

    typedef enum
    {
        VVTK_ANTI_FLICKER_MODE_AUTO = 0,
        VVTK_ANTI_FLICKER_MODE_50_MHZ,
        VVTK_ANTI_FLICKER_MODE_60_MHZ,
    } VVTK_ANTI_FLICKER_MODE;

    typedef enum
    {
        VVTK_DAY_NIGHT_MODE_AUTO = 0,
        VVTK_DAY_MODE,
        VVTK_NIGHT_MODE,
        VVTK_DAY_NIGHT_MODE_AUTO_ALS,
    } VVTK_DAY_NIGHT_MODE;

    typedef enum
    {
        VVTK_DAY_STATE = 0,
        VVTK_NIGHT_STATE,
    } VVTK_DAY_NIGHT_STATE;

    typedef struct
    {
        int brightness;
        int contrast;
        int saturation;
        int sharpness;
        int mirror;
        int flip;
    } vvtk_isp_config_t;

    typedef struct
    {
        int minimum;
        int maximum;
    } vvtk_range_t;

    typedef struct
    {
        vvtk_range_t brightness;
        vvtk_range_t contrast;
        vvtk_range_t saturation;
        vvtk_range_t sharpness;
        vvtk_range_t mirror;
        vvtk_range_t flip;
    } vvtk_isp_config_range_t;

    typedef enum
    {
        VVTK_OSD_TOP_LEFT = 0,
        VVTK_OSD_BOTTOM_LEFT,
        VVTK_OSD_TOP_RIGHT,
        VVTK_OSD_BOTTOM_RIGHT,
        VVTK_OSD_DISABLE,
    } VVTK_OSD_POSITION;

    typedef enum
    {
        VVTK_DNR_DISABLE = 0,
        VVTK_DNR_ENABLE,
    } VVTK_DNR_MODE;

    typedef enum
    {
        VVTK_AWB_MANUAL = 0,
        VVTK_AWB_AUTO,
    } VVTK_AWB_MODE;

    typedef struct
    {
        int value;
        vvtk_range_t range;
    } vvtk_wb_temp_t;

    typedef enum
    {
        VVTK_WDR_DISABLE = 0,
        VVTK_WDR_MANUAL,
        VVTK_WDR_AUTO,
    } VVTK_WDR_MODE;

    typedef struct
    {
        int value;
        vvtk_range_t range;
    } vvtk_wdr_level_t;

    typedef struct
    {
        int width;
        int height;
    } vvtk_window_coordinate_t;

    /**
     * @brief Structure of AE config
     *
     * weight:
     *   0: AE calculation will exclude ae_win region.
     *   1: AE calculation only include ae_win region.
     *   2-255: The weighting used in ae_win region, other region use weighting 1.
     *
     */
    typedef struct
    {
        vvtk_window_coordinate_t   win_coord; /**> The coordinate used by ae_win */
        vvtk_window_config_t       ae_win; /**> Setting the AE window */
        int                        weight; /**> Weighting (0..255)*/
    } vvtk_ae_config_t;

    /**
     * Callback function declaration of the day/night state change
     *
     * @see VVTK_DAY_NIGHT_STATE
     *
     * @param state The current day/night state
     * @return VVTK_RET_CALLBACK stop or continue
     */
    typedef VVTK_RET_CALLBACK (*VVTK_DAYNIGHT_CALLBACK)(VVTK_DAY_NIGHT_STATE state, const void *user_data);

    /**
     * @brief Sets the anti-flicker mode for the camera.
     *
     * This function allows you to set the anti-flicker mode for the camera to reduce
     * flickering in the captured images. It takes a mode parameter of type
     * VVTK_ANTI_FLICKER_MODE to specify the desired mode.
     *
     * @param mode The anti-flicker mode to set.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_anti_flicker(VVTK_ANTI_FLICKER_MODE mode);

    /**
     * @brief Retrieves the current anti-flicker mode of the camera.
     *
     * This function allows you to retrieve the current anti-flicker mode of the camera.
     * It stores the mode in the provided mode pointer.
     *
     * @param mode Pointer to a VVTK_ANTI_FLICKER_MODE variable to store the mode.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_anti_flicker(VVTK_ANTI_FLICKER_MODE *mode);

    /**
     * @brief Retrieves the current ISP (Image Signal Processor) configuration.
     *
     * This function retrieves the current image processing parameters such as
     * brightness, contrast, saturation, sharpness, mirror, and flip. The configuration
     * is stored in the provided config structure.
     *
     * @param config Pointer to a vvtk_isp_config_t structure to store the ISP configuration.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_isp_config(vvtk_isp_config_t *config);

    /**
     * @brief Retrieves the current ISP (Image Signal Processor) configuration range.
     *
     * This function retrieves the current image processing parameters range such as
     * brightness, contrast, saturation, sharpness, mirror, and flip. The configuration
     * range date is stored in the provided structure.
     *
     * @param config_range Pointer to a vvtk_isp_config_range_t structure to store config data range.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_isp_config_range(vvtk_isp_config_range_t *config_range);

    /**
     * @brief Sets the ISP (Image Signal Processor) configuration for the camera.
     *
     * This function allows you to set the image processing parameters such as brightness,
     * contrast, saturation, sharpness, mirror, and flip for the camera. It takes a
     * configuration structure as input.
     *
     * @param config Pointer to a vvtk_isp_config_t structure containing the ISP configuration.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_isp_config(vvtk_isp_config_t *config);

    /**
     * @brief Sets the day/night mode for the camera.
     *
     * This function allows you to set the day/night mode for the camera, which can
     * be set to auto, day mode, or night mode. It takes a mode parameter of type
     * VVTK_DAY_NIGHT_MODE to specify the desired mode.
     *
     * @param mode The day/night mode to set.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_day_night_mode(VVTK_DAY_NIGHT_MODE mode);

    /**
     * @brief Sets the day/night mode for the camera.
     *
     * This function allows you to set the day/night mode for the camera, which can
     * be set to auto, day mode, or night mode. It takes a mode parameter of type
     * VVTK_DAY_NIGHT_MODE to specify the desired mode.
     *
     * @param mode The day/night mode to set.
     * @param cb The callback function when day/night state changed.
     * @param user_data Optional user data to be passed to the callback function.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_day_night_mode_ex(VVTK_DAY_NIGHT_MODE mode, VVTK_DAYNIGHT_CALLBACK cb, const void *user_data);

    /**
     * @brief Release resources for vvtk_set_day_night_mode() or vvtk_set_day_night_mode_ex() APIs
     *
     * This function is responsible to release resources for vvtk_set_day_night_mode() or
     * vvtk_set_day_night_mode_ex() APIs
     *
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_day_night_mode_resource_release();

    /**
     * @brief Retrieves the current day/night mode of the camera.
     *
     * This function allows you to retrieve the current day/night mode of the camera.
     * It stores the mode in the provided mode pointer.
     *
     * @param mode Pointer to a VVTK_DAY_NIGHT_MODE variable to store the mode.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_day_night_mode(VVTK_DAY_NIGHT_MODE *mode);
    /**
     * @brief Sets the position of the on-screen display (OSD) for displaying time.
     *
     * This function allows you to set the position where the time should be displayed
     * on the camera's video feed. The position is specified using the VVTK_OSD_POSITION
     * enum.
     *
     * @param position The position at which the OSD time should be displayed.
     * @return VVTK_RET indicating the success or failure of the operation.
     */

    VVTK_RET vvtk_set_osd_time(VVTK_OSD_POSITION position);
    /**
     * @brief Retrieves the current position of the on-screen display (OSD) for displaying time.
     *
     * This function allows you to retrieve the current position where the time is displayed
     * on the camera's video feed. The current position is stored in the provided position
     * pointer.
     *
     * @param position Pointer to a VVTK_OSD_POSITION variable to store the current OSD position.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_osd_time(VVTK_OSD_POSITION *position);
    /**
     * @brief Sets the position of the on-screen display (OSD) for displaying custom text.
     *
     * This function allows you to set the position where custom text should be displayed
     * on the camera's video feed. The position is specified using the VVTK_OSD_POSITION
     * enum.
     *
     * @param position The position at which the OSD custom text should be displayed.
     * @param text The OSD custom text to displayed.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_osd_text(VVTK_OSD_POSITION position, char *text);
    /**
     * @brief Retrieves the current position of the on-screen display (OSD) for displaying custom text.
     *
     * This function allows you to retrieve the current position where custom text is displayed
     * on the camera's video feed. The current position is stored in the provided position
     * pointer.
     *
     * @param position Pointer to a VVTK_OSD_POSITION variable to store the current OSD position.
     * @param text Pointer to a variable to store the current OSD text.
     * @param text_len The size of the text variable to store the current OSD text.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_osd_text(VVTK_OSD_POSITION *position, char *text, int text_len);
    /**
     * @brief Sets the privacy mask configuration for a specific window.
     *
     * This function allows you to set the privacy mask configuration for a specific window
     * on the camera's video feed. You can specify the index of the window, enable or disable
     * the privacy mask, and provide the window configuration using the vvtk_window_config_t
     * structure. The mask coordinate must be even number, if it is odd number will round 
     * to even internally.
     *
     * @param index The index of the privacy mask window.
     * @param enable Flag to enable or disable the privacy mask for the specified window.
     * @param window The configuration for the privacy mask window.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_privacy_mask(int index, int enable, vvtk_window_config_t *window);
    /**
     * @brief Retrieves the privacy mask configuration for a specific window.
     *
     * This function allows you to retrieve the privacy mask configuration for a specific window
     * on the camera's video feed. You can specify the index of the window, and the enable status
     * and window configuration are stored in the provided pointers.
     *
     * @param index The index of the privacy mask window.
     * @param enable Pointer to an integer to store the enable status of the privacy mask for
     *               the specified window.
     * @param window Pointer to a vvtk_window_config_t structure to store the configuration
     *               for the privacy mask window.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_privacy_mask(int index, int *enable, vvtk_window_config_t *window);

    /**
     * @brief Sets the white balance mode for the camera.
     *
     * This function allows you to set the white balance mode for the camera.
     * It takes a mode parameter of type VVTK_AWB_MODE to specify the desired mode.
     *
     * @param mode The white balance mode to set.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_wb_mode(VVTK_AWB_MODE mode);

    /**
     * @brief Retrieves the current white balance mode of the camera.
     *
     * This function allows you to retrieve the current white balance mode of the camera.
     * It stores the mode in the provided mode pointer.
     *
     * @param mode Pointer to a VVTK_AWB_MODE variable to store the mode.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_wb_mode(VVTK_AWB_MODE *mode);

    /**
     * @brief Sets the white balance temperature for the camera manually.
     *
     * This function allows you to set the white balance temperature for the camera in VVTK_AWB_MANUAL mode.
     *
     * @param wb_temp The white balance temperature to set.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_wb_temperature(vvtk_wb_temp_t *wb_temp);

    /**
     * @brief Retrieves the current white balance temperature of the camera.
     *
     * This function allows you to retrieve the current white balance temperature of the camera.
     *
     * @param wb_temp Pointer to a vvtk_wb_temp_t variable to store the white balance temperature.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_wb_temperature(vvtk_wb_temp_t *wb_temp);

    /**
     * @brief Sets the wdr mode for the camera.
     *
     * This function allows you to set the wdr mode for the camera. It takes a mode parameter of type
     * VVTK_WDR_MODE to specify the desired mode.
     *
     * @param mode The wdr mode to set.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_wdr_mode(VVTK_WDR_MODE mode);

    /**
     * @brief Retrieves the current wdr mode of the camera.
     *
     * This function allows you to retrieve the current wdr mode of the camera.
     * It stores the mode in the provided mode pointer.
     *
     * @param mode Pointer to a VVTK_WDR_MODE variable to store the mode.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_wdr_mode(VVTK_WDR_MODE *mode);

    /**
     * @brief Sets the wdr level for the camera manually.
     *
     * This function allows you to set the wdr level for the camera in VVTK_WDR_MANUAL mode.
     *
     * @param wdr_level The wdr level to set.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_wdr_level(vvtk_wdr_level_t *wdr_level);

    /**
     * @brief Retrieves the current wdr level of the camera.
     *
     * This function allows you to retrieve the current wdr level of the camera.
     *
     * @param wdr_level Pointer to a vvtk_wdr_level_t variable to store the wdr level.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_wdr_level(vvtk_wdr_level_t *wdr_level);

    /**
     * @brief Sets the dnr mode for the camera.
     *
     * This function allows you to set the dnr mode for the camera. It takes a mode parameter
     * of type VVTK_DNR_MODE to specify the desired mode.
     *
     * @param mode The dnr mode to set.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_dnr(VVTK_DNR_MODE mode);

    /**
     * @brief Retrieves the current DNR mode of the camera.
     *
     * This function allows you to retrieve the current DNR mode of the camera.
     * It stores the mode in the provided mode pointer.
     *
     * @param mode Pointer to a VVTK_DNR_MODE variable to store the mode.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_dnr(VVTK_DNR_MODE *mode);

    /**
     * @brief Retrieves the current als threshold and the range of threshold for the camera.
     *
     * This function allows you to retrieves the current als threshold and the range of threshold for the camera.
     *
     * @param threshold Pointer to a integer variable to store the threshold.
     * @param range Pointer to a integer variable to store the range.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_als_threshold(int *threshold, int *range);
    /**
     * @brief Sets the als threshold for the camera.
     *
     * This function allows you to set the als threshold for the camera. You can specify the threshold of the als, the range value of threshold.
     * The range value of threshold is a buffer that switching daynight mode sensitivity.
     *
     * @param threshold The als threshold to set, available value 0~100, default 20.
     * @param range The range value of threshold to set, available value 0~20, default 10.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_als_threshold(int threshold, int range);

    /**
     * Sets the AE (auto exposure) configuration for the camera.
     *
     * @see vvtk_ae_config_t
     *
     * @param config Pointer to a vvtk_ae_config_t structure containing the AE configuration.
     *               After reboot, ae setting will reset to default. You need to reconfig it. 
     *
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_ae_config(vvtk_ae_config_t *config);

    /**
     * @brief Reset AE setting
     *
     * Reset AE setting to default
     *
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_reset_ae();
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __VVTK_CAMERA_H__ */
