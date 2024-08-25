#ifndef __VVTK_HUMAN_DETECT_H__
#define __VVTK_HUMAN_DETECT_H__
#ifdef __cplusplus
extern "C"
{
#endif
#include <vvtk_def.h>

typedef enum
{
    VVTK_TYPE_FACE = 0,
    VVTK_TYPE_PERSON,
} VVTK_HUMAN_DETECT_TYPE;

typedef enum
{
    VVTK_PROB_THREE = 3,
    VVTK_PROB_FOUR,
    VVTK_PROB_FIVE,
    VVTK_PROB_SIX,
    VVTK_PROB_SEVEN,
} VVTK_HUMAN_DETECT_PROB;

typedef struct
{
    int x1;
    int y1;
    int x2;
    int y2;
} VVTK_HD_COORDINATE;


/**
    * Callback function declaration of detected
    *
    * @see VVTK_HUMAN_DETECT_TYPE
    *
    * @param coordinate The coordinate of detected
    * @return VVTK_RET_CALLBACK stop or continue
    */
typedef VVTK_RET_CALLBACK (*VVTK_HUMANDETECT_CALLBACK)(VVTK_HD_COORDINATE coordinate, const void *user_data);

/**
    * @brief Enable human detect function with tracking face or person.
    *
    *
    * @param VVTK_HUMAN_DETECT_TYPE 0:face, 1:person
    * @param VVTK_HUMAN_DETECT_PROB accurate prob, default VVTK_PROB_THREE, means 30% accurate probability
    * @param VVTK_HD_COORDINATE set detect frame, default max, left-top (x1,y1)[(0,0)] to right-bottom (x2,y2)[(256,160)]
    * @param cb The callback function when human detected.
    * @param user_data Optional user data to be passed to the callback function.
    * @return VVTK_RET indicating the success or failure of the operation.
    */
VVTK_RET vvtk_set_human_detect(VVTK_HUMAN_DETECT_TYPE type,VVTK_HUMAN_DETECT_PROB prob, VVTK_HD_COORDINATE *frame, VVTK_HUMANDETECT_CALLBACK cb, const void *user_data);

/**
    * @brief Release resources for vvtk_set_human_detect() APIs
    *
    * This function is responsible to release resources for vvtk_set_human_detect() APIs
    *
    * @return VVTK_RET indicating the success or failure of the operation.
    */
VVTK_RET vvtk_human_detect_resource_release();
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __VVTK_HUMAN_DETECT_H__ */
