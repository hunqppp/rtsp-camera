#ifndef __VVTK_DEF_H__
#define __VVTK_DEF_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t VVTK_BYTE;

typedef enum
{
    VVTK_RET_SUCCESS = 0,
    VVTK_RET_ERR_UNKNOWN,         // 1 Unknown Error
    VVTK_RET_ERR_ACCESS,          // 2 Permission denied
    VVTK_RET_ERR_NO_MEM,          // 3 Not enough memory
    VVTK_RET_ERR_NO_SPACE,        // 4 Not enough space
    VVTK_RET_ERR_IO,              // 5 Other I/O error
    VVTK_RET_ERR_ARG,             // 6 Invalid argument of some other kind
    VVTK_RET_ERR_TIMEOUT,         // 7 Timeout
    VVTK_RET_ERR_NOT_INITIALIZED, // 8 Internal resource not initialized
    VVTK_RET_ERR_NOT_SUPPORTED,   // 9 Feature not supported
} VVTK_RET;

typedef enum
{
    VVTK_RET_CALLBACK_STOP = 0,
    VVTK_RET_CALLBACK_CONTINUE,
} VVTK_RET_CALLBACK;

typedef struct
{
    int x;
    int y;
} vvtk_point_t;

typedef struct
{
    vvtk_point_t left_top;
    vvtk_point_t right_bottom;
} vvtk_window_config_t;

#ifdef __cplusplus
}
#endif

#endif /* __VVTK_DEF_H__ */
