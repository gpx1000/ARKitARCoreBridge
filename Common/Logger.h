//
// Created by Steven Winston on 1/30/18.
//

#ifndef ARKITARCOREBRIDGE_LOGGER_H
#define ARKITARCOREBRIDGE_LOGGER_H

#include <string>

#define STR(x) #x
#define STRINGIFY(x) STR(x)
#define TAG "Log:" __FILE__ ":" STRINGIFY(__LINE__)

#ifdef __ANDROID__
#include <android/log.h>

#define ERROR(...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))

#ifdef NDEBUG
#define INFO(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define WARN(...) ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))
#define VERBOSE(...) ((void)__android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__))
#endif
#endif

#ifdef __APPLE__
#include <os/log.h>

#define ERROR(...) (os_log_with_type(OS_LOG_DEFAULT, OS_LOG_TYPE_ERROR, __VA_ARGS__))

#ifdef NDEBUG
#define INFO(...) (os_log_with_type(OS_LOG_DEFAULT, OS_LOG_TYPE_INFO, __VA_ARGS__))
#define WARN(...) (os_log_with_type(OS_LOG_DEFAULT, OS_LOG_TYPE_DEBUG, __VA_ARGS__))
#define VERBOSE(...) (os_log_with_type(OS_LOG_DEFAULT, OS_LOG_TYPE_DEFAULT, __VA_ARGS__))
#endif
#endif

#ifndef NDEBUG
#define INFO
#define WARN
#define VERBOSE
#endif


#endif //ARKITARCOREBRIDGE_LOGGER_H
