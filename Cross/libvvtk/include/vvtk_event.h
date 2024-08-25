#ifndef __VVTK_EVENT_H__
#define __VVTK_EVENT_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <vvtk_def.h>
#include <vvtk_video.h>
#define ROI_DIM	8
    /**
     * @brief Structure representing the types of regions of interest (ROI) supported.
     *
     * This structure defines whether square and polygon ROIs are supported.
     */
    typedef struct
    {
        bool square;
        bool polygon;
		bool grid;
    } vvtk_roi_type_t;

    /**
     * @brief Structure representing the capabilities of ROIs.
     *
     * This structure specifies the number of ROIs and the types of ROIs supported.
     */
    typedef struct
    {
        int roi_num;
        vvtk_roi_type_t type;
    } vvtk_roi_capability_t;

    /**
     * @brief Structure representing a region of interest (ROI).
     *
     * This structure contains information about an ROI, including a 2-D roi array
	 * and a ratio used to determine motion events.
     */
    typedef struct
    {
		union {
			vvtk_point_t *point_list;
			int point_size;
			char roi_grid[ROI_DIM];
		};
		int sensitivity; // The threshold ratio that determined motion event
    } vvtk_roi_t;

    /**
     * @brief Structure representing information about motion within an ROI.
     *
     * This structure contains information about motion within a specific region of interest (ROI),
     * including the ROI index and a ratio that describes the threshold ratio that determined a
     * motion event.
     */
    typedef struct
    {
        int roi_index; /**< Index of the ROI where motion was detected. */
        int ratio;     /**< The threshold ratio that determined a motion event. */
    } vvtk_motion_info_t;

    typedef struct
    {
        int interval; // ms
    } vvtk_motion_config_t;

    /**
     * @brief Structure representing information about a motion event.
     *
     * This structure contains details about a motion event, including a timestamp,
     * an associated image, the size of motion info, and a list of motion information.
     */
    typedef struct
    {
        time_t timestamp;
        vvtk_jpeg_image_t *image;
        int motion_info_size;
        vvtk_motion_info_t *motion_info_list;
    } vvtk_motion_event_t;

    /**
     * @brief Callback function for handling motion events.
     *
     * This function type defines the callback function that will be called when
     * a motion event occurs.
     *
     * @param event Pointer to the motion event data.
     */
    typedef void (*VVTK_MOTION_EVENT_CALLBACK)(const vvtk_motion_event_t *event);

    /**
     * @brief Retrieves the capability of supported ROIs.
     *
     * This function retrieves information about the types of ROIs that are supported.
     *
     * @param capability Pointer to a vvtk_roi_capability_t structure to store the capability.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_roi_capability(vvtk_roi_capability_t *capability);

    /**
     * @brief Retrieves the list of ROIs.
     *
     * This function retrieves the list of defined ROIs and their configurations.
     *
     * @param list Pointer to a vvtk_roi_t array to store the list of ROIs.
     * @param size Pointer to an integer to store the size of the ROI list.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_roi_list(vvtk_roi_t **list, int *size);

    /**
     * @brief Retrieves the indexies of ROI grids.
     *
     * This function retrieves the indexies of defined ROI grids.
     *
     * @param list Pointer to a vvtk_roi_t to stores the indexies of ROI grids.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_roi_grid(vvtk_roi_t *list);

    /**
     * @brief Sets the list of ROIs.
     *
     * This function allows you to set the list of ROIs and their configurations.
     *
     * @param list Pointer to a vvtk_roi_t array containing the list of ROIs.
     * @param size The size of the ROI list.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_roi_list(const vvtk_roi_t *list, int size);
	
	/**
     * @brief Sets the grid of ROIs.
     *
     * This function allows you to set the grid of ROIs and their configurations.
     *
     * @param list Pointer to the indexies of enabled ROI grid.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_roi_grid(const vvtk_roi_t *grid_index);

    /**
     * @brief Sets the configuration for motion events.
     *
     * This function allows you to configure motion event settings.
     *
     * @param config Pointer to a vvtk_motion_config_t structure containing the configuration.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_motion_config(const vvtk_motion_config_t *config);

    /**
     * @brief Retrieves the configuration for motion events.
     *
     * This function retrieves the current configuration for motion events.
     *
     * @param config Pointer to a vvtk_motion_event_t structure to store the configuration.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_get_motion_config(vvtk_motion_config_t *config);

    /**
     * @brief Sets the callback function for handling motion events.
     *
     * This function allows you to register a callback function to handle motion events.
     *
     * @param cb Pointer to the callback function.
     * @return VVTK_RET indicating the success or failure of the operation.
     */
    VVTK_RET vvtk_set_motion_event_callback(VVTK_MOTION_EVENT_CALLBACK cb);
	
	/**
     * @brief Stop Motion Detection.
     *
     * This function allows you to stop motion detection thread.
     *
     * @return VVTK_RET indicating the success or failure of the operation.
     */

	VVTK_RET vvtk_stop_motion_detection(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __VVTK_EVENT_H__ */
