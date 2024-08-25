#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdio.h>

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#define LOG_PRINT_EN    0
#define LOG_DEBUG_EN    0
#define LOG_ERROR_EN    0
#define LOG_WARNINNG_EN 0
#define LOG_RECEIVED_EN 0
#define LOG_SEND_EN     0

#if LOG_PRINT_EN
#define LOG_PRINT(fmt, ...)         printf(fmt "\r\n", ##__VA_ARGS__)
#else
#define LOG_PRINT(fmt, ...)
#endif

#if LOG_DEBUG_EN
#define LOG_DEBUG(fmt, ...)			printf(KGRN fmt KNRM "\r\n", ##__VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...)
#endif

#if LOG_ERROR_EN
#define LOG_ERROR(fmt, ...)			printf(KRED fmt KNRM "\r\n", ##__VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...)
#endif

#if LOG_WARNINNG_EN
#define LOG_WARNING(fmt, ...)		printf(KYEL fmt KNRM "\r\n", ##__VA_ARGS__)
#else
#define LOG_WARNING(fmt, ...)
#endif

#if LOG_WARNINNG_EN
#define LOG_RECEIVED(fmt, ...)      printf(KGRN fmt KNRM "\r\n", ##__VA_ARGS__)
#else
#define LOG_RECEIVED(fmt, ...)
#endif

#if LOG_SEND_EN
#define LOG_SEND(fmt, ...)          printf(KYEL fmt KNRM "\r\n", ##__VA_ARGS__)
#else
#define LOG_SEND(fmt, ...)
#endif

#endif /* __DEBUG_H */