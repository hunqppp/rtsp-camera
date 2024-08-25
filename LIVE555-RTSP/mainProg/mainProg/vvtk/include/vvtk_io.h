#ifndef __VVTK_IO_H__
#define __VVTK_IO_H__
#ifdef __cplusplus
extern "C"
{
#endif
#include <vvtk_def.h>
    typedef enum
    {
        VVTK_SD_STATUS_INSERT = 0,
        VVTK_SD_STATUS_NOCARD = 1
    } VVTK_SD_STATUS;

    typedef enum
    {
        VVTK_BTN_TYPE_RESET = 0,
    } VVTK_BTN_TYPE;

    typedef enum
    {
        VVTK_BTN_STATUS_UNKNOWN = 0,
        VVTK_BTN_STATUS_PRESS,
        VVTK_BTN_STATUS_RELEASE,
    } VVTK_BTN_STATUS;

    typedef enum
    {
        VVTK_GPIO_UNKNOWN = 0,
        VVTK_GPIO_IRCUT,
        VVTK_GPIO_IRLED,
    } VVTK_GPIO_NAME;

    typedef enum
    {
        VVTK_GPIO_OFF = 0,
        VVTK_GPIO_ON,
    } VVTK_GPIO_STATUS;

    typedef enum
    {
        VVTK_LED_COLOR_UNKNOWN = 0,
        VVTK_LED_COLOR_RED,
        VVTK_LED_COLOR_GREEN,
        VVTK_LED_COLOR_MAX
    } VVTK_LED_COLOR;

    typedef enum
    {
        VVTK_LED_MODE_OFF = 0,
        VVTK_LED_MODE_ON,
        VVTK_LED_MODE_BLINKING,
    } VVTK_LED_MODE;

    typedef enum
    {
        VVTK_LIGHTING_LED_WHITE=0,
        VVTK_LIGHTING_LED_IR
    } VVTK_LIGHTING_LED_TYPE;

    typedef enum
    {
        VVTK_MOTOR_PAN = 0,
        VVTK_MOTOR_TILT,
        VVTK_MOTOR_ZOOM,
    } VVTK_MOTOR_TYPE;

    typedef enum
    {
        VVTK_MOTOR_DIRECTION_UNKNOWN = 0,
        VVTK_MOTOR_DIRECTION_LEFT,   /* PAN Motor*/
        VVTK_MOTOR_DIRECTION_RIGHT,  /* PAN Motor*/
        VVTK_MOTOR_DIRECTION_UP,     /* TILT Motor*/
        VVTK_MOTOR_DIRECTION_DOWN,   /* TILT Motor*/
        VVTK_MOTOR_DIRECTION_IN,     /* ZOOM Motor*/
        VVTK_MOTOR_DIRECTION_OUT,    /* ZOOM Motor*/
        VVTK_MOTOR_DIRECTION_ORIGIN, /**< Should stop at origin. */
    } VVTK_MOTOR_DIRECTION;

    typedef enum
    {
        VVTK_MOTOR_STATUS_UNKNOWN = 0,
        VVTK_MOTOR_STATUS_PANNING,
        VVTK_MOTOR_STATUS_STOP,
    } VVTK_MOTOR_STATUS;

    typedef struct
    {
        VVTK_GPIO_NAME name;
        VVTK_GPIO_STATUS status;
    } vvtk_gpio_status_t;

    typedef struct
    {
        VVTK_LED_COLOR color;
        VVTK_LED_MODE mode;
        int interval; // blinking interval in ms
    } vvtk_led_status_t;

    typedef struct
    {
        VVTK_MOTOR_DIRECTION direction;
        int speed; /**< 0: Stop, 1-100: Pan seed. */
        int step;  /**< 0: Continuously pan, 1-N: Motor specific steps. */
    } vvtk_motor_action_t;

    typedef struct
    {
        VVTK_MOTOR_STATUS status;
        int step; /**< Number of steps to the origin */
    } vvtk_motor_status_t;

    /**
     * @brief Enumeration for supported file system types.
     */
    typedef enum
    {
        VVTK_FS_UNKNOWN, /**< Unknown file system type. */
        VVTK_FS_FAT,     /**< FAT file system type. */
        VVTK_FS_EXT4,    /**< ext4 file system type. */
        // Add more file system types as needed
    } VVTK_FILESYSTEM_TYPE;

    /**
     * @brief Structure to represent SD card information.
     */
    typedef struct
    {
        uint64_t total_size;         /**< Total size of the SD card in bytes. */
        uint64_t remaining_capacity; /**< Remaining capacity on the SD card in bytes. */
    } vvtk_sdcard_info;

    typedef void (*VVTK_SD_CALLBACK)(VVTK_SD_STATUS status, const void *user_data);

    /**
     * Callback function declation of the button usage
     *
     * @param type The type of the button
     * @param status The button status
     */
    typedef void (*VVTK_BTN_CALLBACK)(VVTK_BTN_TYPE type, VVTK_BTN_STATUS status);
    /**
     * Get the button status
     *
     * @param type The button to be queried
     * @param status The button status
     * @return VVTK_RET_SUCCESS without error(s)
     */
    VVTK_RET vvtk_get_button_status(VVTK_BTN_TYPE type, VVTK_BTN_STATUS *status);

    /**
     * Immediately and continuously report button status events
     *
     * @param type The button to be queried
     * @param callback The callback function when detecting the button status change
     * @return FBM_RET_SUCCESS without error(s)
     */
    VVTK_RET vvtk_set_button_status_callback(VVTK_BTN_TYPE type, VVTK_BTN_CALLBACK cb);

    /**
     * Get the current LED mode and current color
     * @param config the LED settings
     * @return VVTK_RET_SUCCESS when get LED okay
     */
    VVTK_RET vvtk_get_led_status(vvtk_led_status_t *status);

    /**
     * @brief Set the LED status.
     *
     * This function sets the LED color and mode.
     *
     * @param status The desired LED status.
     * @return VVTK_RET_SUCCESS if successful, error code otherwise.
     */

    VVTK_RET vvtk_set_led_status(vvtk_led_status_t status);

    /**
     * Callback function declation of the Motor usage
     *
     * @param status The Motor status
     */
    typedef void (*VVTK_MOTOR_CALLBACK)(vvtk_motor_status_t status, const void *user_data);

    /**
     * @brief Set a callback for motor status events.
     *
     * This function registers a callback function to be called when the status of
     * a motor changes.
     *
     * @param callback The callback function to handle motor status changes.
     * @param user_data Optional user data to be passed to the callback function.
     * @return VVTK_RET_SUCCESS if successful, error code otherwise.
     */
    VVTK_RET vvtk_set_motor_status_callback(VVTK_MOTOR_TYPE type, VVTK_MOTOR_CALLBACK cb, const void *user_data);

    /**
     * @brief Disable a callback of motor .
     *
     * This function disable a callback function of motor which be registered.
     *
     * @param type The type of motor.
     * @return VVTK_RET_SUCCESS if successful, error code otherwise.
     */
    VVTK_RET vvtk_disable_motor_status_callback(VVTK_MOTOR_TYPE type);

    /**
     * @brief Get the maximum number of steps for a motor type.
     *
     * This function retrieves the maximum number of steps a specific type of motor can perform.
     *
     * @param type The type of motor to query.
     * @param max_steps Pointer to store the maximum steps.
     * @return VVTK_RET_SUCCESS if successful, error code otherwise.
     */
    VVTK_RET vvtk_get_motor_max_steps(VVTK_MOTOR_TYPE type, int *max_steps);

    /**
     * @brief Control a motor's movement.
     *
     * This function allows you to control the movement of a specific motor type
     * with the provided action settings.
     *
     * @param type The type of motor to control.
     * @param action The motor action settings.
     * @return VVTK_RET_SUCCESS if successful, error code otherwise.
     */
    VVTK_RET vvtk_motor(VVTK_MOTOR_TYPE type, const vvtk_motor_action_t *action);
    /**
     * Initialize the Motor driver and application.
     *
     * @return VVTK_RET_SUCCESS if the motor library is initialized successfully,
     *         or an error code if there are any issues during initialization.
     */
    VVTK_RET vvtk_motor_init();

    /**
     * @brief Set a callback function to receive notifications on SD card insertion status.
     *
     * This function allows the application to register a callback function that will be
     * invoked when the SD card insertion status changes. The callback function will receive
     * a VVTK_SD_STATUS parameter indicating whether an SD card has been inserted or if
     * no card is present.
     *
     * @param cb A pointer to a callback function with the signature:
     *           `void callback(VVTK_SD_STATUS status)`
     *           The callback function will be called with the current SD card status.
     *
     * @return VVTK_RET_SUCCESS if the callback is successfully registered.
     *         VVTK_RET_ERROR if there is an issue registering the callback.
     *
     * @note The callback function should handle the SD card status appropriately based on
     *       the received VVTK_SD_STATUS parameter.
     * @note To unregister the callback, call this function with a NULL pointer as the argument.
     */

    VVTK_RET vvtk_set_sd_insert_callback(VVTK_SD_CALLBACK cb, const void *user_data);

    /**
     * @brief Format the SD card with the specified file system type.
     *
     * This function allows the application to format the SD card using a specified
     * file system type, such as FAT or ext4. It is crucial to choose the appropriate
     * file system based on the requirements and compatibility with the target system.
     *
     * @param type The file system type to be applied to the SD card.
     *             Supported file system types include constants like VVTK_FS_FAT
     *             and VVTK_FS_EXT4, indicating FAT and ext4 formatting, respectively.
     *
     * @return VVTK_RET_SUCCESS if the SD card formatting is successful.
     *         VVTK_RET_ERROR   if an error occurs during the formatting process.
     *
     * @note This operation will erase all data on the SD card. Ensure that the
     *       application has the necessary permissions and user confirmation
     *       before invoking this function.
     */
    VVTK_RET vvtk_sdcard_format(VVTK_FILESYSTEM_TYPE type);

    /**
     * @brief Retrieve information about the SD card.
     *
     * This function retrieves essential information about the SD card, such as its
     * total size and remaining capacity, and populates the provided vvtk_sdcard_info
     * structure with the obtained data.
     *
     * @param sdinfo Pointer to a vvtk_sdcard_info structure to store the SD card information.
     *               The structure should be allocated by the caller.
     *
     * @return VVTK_RET_SUCCESS if the SD card information is successfully retrieved.
     *         VVTK_RET_ERROR if there is an error during the retrieval process.
     *
     * @note The caller is responsible for allocating memory for the vvtk_sdcard_info structure.
     * @note Ensure that the SD card is accessible and properly mounted before invoking this function.
     */
    VVTK_RET vvtk_get_sdcard_info(vvtk_sdcard_info *sdinfo);

/**
 * @brief Get the range of lighting values supported for LED control.
 *
 * This function retrieves the minimum and maximum values that can be set for LED lighting.
 *
 * @param[out] min Pointer to store the minimum lighting value.
 * @param[out] max Pointer to store the maximum lighting value.
 * @return VVTK_RET_SUCCESS if successful, error code otherwise.
 */
VVTK_RET vvtk_get_led_lighting_range(VVTK_LIGHTING_LED_TYPE type,int *min, int *max);

/**
 * @brief Set the lighting value for a specific type of LED.
 *
 * This function sets the lighting value for a particular type of LED (e.g., white or infrared).
 *
 * @param type The type of LED for which the lighting value is being set.
 * @param value The lighting value to set for the specified LED type.
 * @return VVTK_RET_SUCCESS if successful, error code otherwise.
 */
VVTK_RET vvtk_set_led_lighting(VVTK_LIGHTING_LED_TYPE type, int value);

/**
 * @brief Get the lighting value for a specific type of LED using a pointer.
 *
 * This function Gets the lighting value for a particular type of LED (e.g., white or infrared)
 * using a pointer to the value.
 *
 * @param type The type of LED for which the lighting value is being set.
 * @param[out] value Pointer to the lighting value to set for the specified LED type.
 * @return VVTK_RET_SUCCESS if successful, error code otherwise.
 */
VVTK_RET vvtk_get_led_lighting(VVTK_LIGHTING_LED_TYPE type, int *value);

/**
 * @brief Retrieves the value from the light sensor.
 *
 * This function, vvtk_get_light_sensor, is used to obtain the current reading
 * from the light sensor. The result, representing light intensity, is stored in
 * the provided integer pointer. The value is scaled between 0 and 100,
 * indicating the intensity percentage.
 *
 * @param value Pointer to an integer where the light sensor reading (0 to 100) will be stored.
 * @return VVTK_RET status code indicating the success or failure of the operation.
 */
VVTK_RET vvtk_get_light_sensor(int *value);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
