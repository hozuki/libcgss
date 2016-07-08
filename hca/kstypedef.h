#ifndef KAWASHIMA_KSTYPEDEF_H
#define KAWASHIMA_KSTYPEDEF_H

#if defined _WIN32 || defined __CYGWIN__
#ifdef BUILDING_DLL
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllexport))
#else
#define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#endif
#else
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllimport))
#else
#define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#endif
#endif
#define DLL_LOCAL
#else
#if __GNUC__ >= 4
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define DLL_PUBLIC
#define DLL_LOCAL
#endif
#endif

#define EXTERN_C extern "C"
#define STDCALL __stdcall

#define KS_API EXTERN_C DLL_PUBLIC STDCALL

typedef signed long long int64;
typedef unsigned long long uint64;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed char int8;
typedef unsigned char uint8;

typedef uint32 ubool;

#ifndef TRUE
#define TRUE ((ubool)1)
#endif
#ifndef FALSE
#define FALSE ((ubool)0)
#endif

typedef enum {
    KS_OP_HAS_MORE_DATA = 1,
    KS_ERR_OK = 0,
    KS_ERR_INVALID_HANDLE = (int32)-1,
    KS_ERR_MAGIC_NOT_MATCH = (int32)-2,
    KS_ERR_ALREADY_CLOSED = (int32)-3,
    KS_ERR_INVALID_PARAMETER = (int32)-4,
    KS_ERR_INVALID_STAGE = (int32)-5,
    KS_ERR_FILE_OP_FAILED = (int32)-6,
    KS_ERR_INVALID_FILE_PROP = (int32)-7,
    KS_ERR_INVALID_OPERATION = (int32)-8,
    KS_ERR_DECODE_FAILED = (int32)-9,
    KS_ERR_BUFFER_TOO_SMALL = (int32)-10,
    KS_ERR_CHECKSUM_NOT_MATCH = (int32)-11,
    KS_ERR_INVALID_INTERNAL_STATE = (int32)-12,
    KS_ERR_ATH_INIT_FAILED = (int32)-13,
    KS_ERR_CIPH_INIT_FAILED = (int32)-14,
    KS_ERR_STATE_OUT_OF_RANGE = (int32)-15,
    KS_ERR_NOT_IMPLEMENTED = (int32)-16,
    KS_ERR_DECODE_ALREADY_COMPLETED = (int32)-17,
    KS_ERR_FORCE_DWORD = (int32)0xffffffff
} KS_RESULT;

#define KS_CALL_SUCCESSFUL(x) ((ubool)(((int32)x) >= 0))

#endif //KAWASHIMA_COMMON_H
