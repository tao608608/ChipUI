/*****************************************************************************\
*    Program:		bgapi2 - Baumer GenTL consumer
*    Purpose:       Baumer GenTL consumer definitions
*
*    Created:		13.04.2011
*    Author:		Silvio Voitzsch
*    Copyright:		Baumer Optronic GmbH, Radeberg
\*****************************************************************************/
#ifndef __BGAPI2_DEF_H__
#define __BGAPI2_DEF_H__

#if !defined(_GNULINUX)
#ifdef BGAPI2_GENICAM_EXPORTS
#define BGAPI2_DECL __declspec(dllexport)
#else
#define BGAPI2_DECL __declspec(dllimport)
#endif
#else
#define BGAPI2_DECL
#endif
#if defined(__BORLANDC__) || (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
    #define BGAPI2CALL __stdcall
#else
    #define BGAPI2CALL
#endif

#include "bgapi2_featurenames.h"

#ifndef _GNULINUX
typedef  __int64            bo_int64;
typedef unsigned __int64    bo_uint64;
typedef __int32             bo_int;
typedef unsigned __int32    bo_uint;
typedef __int16             bo_short;
typedef unsigned __int16    bo_ushort;
typedef __int8              bo_char;
typedef unsigned __int8     bo_uchar;
typedef unsigned __int16	bo_uint16;

#else
#include <iostream>
#include <inttypes.h>
/* workaround for g++-64bit int64_t == long int and not long long int !!! */
typedef long long int		 bo_int64;
typedef unsigned long long int	 bo_uint64;
//typedef int64_t                bo_int64;
//typedef uint64_t               bo_uint64;
typedef long long int            bo_int64;
typedef unsigned long long int   bo_uint64;
typedef int32_t	            bo_int;
typedef uint32_t            bo_uint;
typedef int16_t	            bo_short;
typedef uint16_t            bo_ushort;
typedef int8_t	            bo_char;
typedef uint8_t	            bo_uchar;
#endif
typedef char*               bo_string;
#ifdef __cplusplus
     typedef bool bo_bool;
#else
     typedef bo_uchar bo_bool;
#endif
typedef double              bo_double;

class CEventPnPObj;
class CEventDeviceEventObj;

#ifdef __cplusplus
namespace BGAPI2
{
    struct _sSystemListData;
    typedef struct BGAPI2::_sSystemListData tSystemListData, *ptSystemListData;

    struct _sInterfaceListData;
    typedef struct BGAPI2::_sInterfaceListData tInterfaceListData, *ptInterfaceListData;

    struct _sSystemData;
    typedef struct BGAPI2::_sSystemData tSystemData, *ptSystemData;

    struct _sDeviceListData;
    typedef struct BGAPI2::_sDeviceListData tDeviceListData, *ptDeviceListData;

    struct _sDataStreamListData;
    typedef struct BGAPI2::_sDataStreamListData tDataStreamListData, *ptDataStreamListData;

    struct _sBufferListData;
    typedef struct BGAPI2::_sBufferListData tBufferListData, *ptBufferListData;

    struct _sBufferData;
    typedef struct BGAPI2::_sBufferData tBufferData, *ptBufferData;

    struct _sDataStreamData;
    typedef struct BGAPI2::_sDataStreamData tDataStreamData, *ptDataStreamData;

    struct _sDeviceData;
    typedef struct BGAPI2::_sDeviceData tDeviceData, *ptDeviceData;

    struct _sInterfaceData;
    typedef struct BGAPI2::_sInterfaceData tInterfaceData, *ptInterfaceData;

    struct _sImageData;
    typedef struct BGAPI2::_sImageData tImageData, *ptImageData;

    struct _sImageProcessorData;
    typedef struct BGAPI2::_sImageProcessorData tImageProcessorData, *ptImageProcessorData;

class BGAPI2_DECL String
{
    public:
        String();
        String( const char * text );
        String( const char * text, int length );
        String( const char & ch,  int length );
        String( const String & Obj );
        String( const char & ch );
        ~String(); 
        
        operator char *();
        operator const char *();
        bool operator == (const char * text);
        bool operator == ( String & ExStr ) const;
        bool operator != (const char * text);
        bool operator != ( const String & ExStr );
        bool operator < ( const String & ExStr ) const;
        const String & operator = (char &);
        const String & operator = (const char *);
        const String & operator = ( const String & ExStr );

#ifdef _GNULINUX
        friend std::ostream& operator << ( std::ostream& out, BGAPI2::String const &ExStr );
#endif
        const char* get() const;
        //        const char* get();

        void set( const char * text );
        int size();
        
    private:
        void* data;
};

#ifdef _GNULINUX
std::ostream& operator << ( std::ostream& out, BGAPI2::String const &ExStr );
#endif


class System;
class BGAPI2_DECL _pairs
{
public:
    String first;
    System* second;
};

class Interface;
class BGAPI2_DECL _pairi
{
public:
    String first;
    Interface* second;
};

class Device;
class BGAPI2_DECL _paird
{
public:
    String first;
    Device* second;
};

class DataStream;
class BGAPI2_DECL _pairds
{
public:
    String first;
    DataStream* second;
};

class Buffer;
class BGAPI2_DECL _pairb
{
public:
    String first;
    Buffer* second;
};

class Node;
class BGAPI2_DECL _pairn
{
public:
    bo_int64 first;
    Node* second;
};

class BGAPI2_DECL _pairnm
{
public:
    String first;
    _pairn second;
};

#ifndef _GNULINUX
#pragma pack (push, 8)
typedef struct  {
    bo_ushort *pcRed;	  // pointer to hist array offered by user, can be zero
    bo_ushort *pcGreen;	  // pointer to hist array offered by user, can be zero
    bo_ushort *pcBlue;	  // pointer to hist array offered by user, can be zero
    bo_ushort *pcLuma;	  // pointer to hist array offered by user, can be zero
    int   length;
    int * pSizeFilled; // pointer to fill info for user
    bool ThresEnable;  
    int  ThresMin;
    int  ThresMax;
	int* red_under;
	int* red_over;
	int* green_under;
	int* green_over;
	int* blue_under;
	int* blue_over;

} bo_tHistRecords;

/**
\struct	tRGB16QUAD
\brief  The tRGB16QUAD structure specifies the information for one color look up table entry
\brief  according to RGBQUAD of WinGdi
*/
typedef struct  {
    bo_ushort rgbBlue; 
    bo_ushort rgbGreen; 
    bo_ushort rgbRed; 
    bo_ushort rgbReserved; 

} bo_tRGB16QUAD;
#pragma pack (pop)
#else
// TODO Linux packed
typedef struct  {
    bo_ushort *pcRed;	  // pointer to hist array offered by user, can be zero
    bo_ushort *pcGreen;	  // pointer to hist array offered by user, can be zero
    bo_ushort *pcBlue;	  // pointer to hist array offered by user, can be zero
    bo_ushort *pcLuma;	  // pointer to hist array offered by user, can be zero
    int   length;
    int * pSizeFilled; // pointer to fill info for user
    bool ThresEnable;  
    int  ThresMin;
    int  ThresMax;
	int*  red_under;
	int*  red_over;
	int*  green_under;
	int*  green_over;
	int*  blue_under;
	int*  blue_over;

} bo_tHistRecords;

/**
\struct	tRGB16QUAD
\brief  The tRGB16QUAD structure specifies the information for one color look up table entry
\brief  according to RGBQUAD of WinGdi
*/
typedef struct  {
    bo_ushort rgbBlue; 
    bo_ushort rgbGreen; 
    bo_ushort rgbRed; 
    bo_ushort rgbReserved; 

} bo_tRGB16QUAD;
#endif
}
#endif

enum BGAPI2_RESULT_LIST
{
    BGAPI2_RESULT_SUCCESS             = 0,
    BGAPI2_RESULT_ERROR               = -1001,
    BGAPI2_RESULT_NOT_INITIALIZED     = -1002,
    BGAPI2_RESULT_NOT_IMPLEMENTED     = -1003,
    BGAPI2_RESULT_RESOURCE_IN_USE     = -1004,
    BGAPI2_RESULT_ACCESS_DENIED       = -1005,
    BGAPI2_RESULT_INVALID_HANDLE      = -1006,
    BGAPI2_RESULT_NO_DATA             = -1008,
    BGAPI2_RESULT_INVALID_PARAMETER   = -1009,
    // GC_ERR_IO                         = -1010,
    BGAPI2_RESULT_TIMEOUT             = -1011,
    BGAPI2_RESULT_ABORT               = -1012,
    BGAPI2_RESULT_INVALID_BUFFER      = -1013,
    BGAPI2_RESULT_NOT_AVAILABLE       = -1014,
    // GC_ERR_INVALID_ADDRESS            = -1015, /* GenTL v1.3 */
    // GC_ERR_BUFFER_TOO_SMALL           = -1016, /* GenTL v1.4 */
    // GC_ERR_INVALID_INDEX              = -1017, /* GenTL v1.4 */
    // GC_ERR_PARSING_CHUNK_DATA         = -1018, /* GenTL v1.4 */
    BGAPI2_RESULT_OBJECT_INVALID      = -1098,
    BGAPI2_RESULT_LOWLEVEL_ERROR      = -1099
};

typedef bo_int BGAPI2_RESULT;

#define BGAPI2_PAYLOADTYPE_UNKNOWN "Unknown"
#define BGAPI2_PAYLOADTYPE_IMAGE "Image"					
#define BGAPI2_PAYLOADTYPE_RAW_DATA "RawData"
#define BGAPI2_PAYLOADTYPE_FILE "File"
#define BGAPI2_PAYLOADTYPE_CHUNK_DATA "ChunkData"
#define BGAPI2_PAYLOADTYPE_CUSTOM_ID "CustomID_1000"

#define BGAPI2_PAYLOADTYPE_IMAGE_EXT "ImageExt"		/* PAYLOADTYPE_IMAGE_EXT */
//other custom id's were delivered in the following format CustomID_xxxx starting with CustomID_1000

#define BGAPI2_NODEINTERFACE_CATEGORY "ICategory"
#define BGAPI2_NODEINTERFACE_INTEGER "IInteger"
#define BGAPI2_NODEINTERFACE_REGISTER "IRegister"
#define BGAPI2_NODEINTERFACE_BOOLEAN "IBoolean"
#define BGAPI2_NODEINTERFACE_COMMAND "ICommand"
#define BGAPI2_NODEINTERFACE_FLOAT "IFloat"
#define BGAPI2_NODEINTERFACE_ENUMERATION "IEnumeration"
#define BGAPI2_NODEINTERFACE_STRING "IString"
#define BGAPI2_NODEINTERFACE_PORT "IPort"

#define BGAPI2_NODEVISIBILITY_BEGINNER "Beginner"
#define BGAPI2_NODEVISIBILITY_EXPERT "Expert"
#define BGAPI2_NODEVISIBILITY_GURU "Guru"
#define BGAPI2_NODEVISIBILITY_INVISIBLE "Invisible"

#define BGAPI2_NODEACCESS_READWRITE "RW"
#define BGAPI2_NODEACCESS_READONLY "RO"
#define BGAPI2_NODEACCESS_WRITEONLY "WO"
#define BGAPI2_NODEACCESS_NOTAVAILABLE "NA"
#define BGAPI2_NODEACCESS_NOTIMPLEMENTED "NI"

#define BGAPI2_NODEREPRESENTATION_LINEAR "Linear"           //valid for BGAPI2_NODEINTERFACE_INTEGER, BGAPI2_NODEINTERFACE_FLOAT
#define BGAPI2_NODEREPRESENTATION_LOGARITHMIC "Logarithmic" //valid for BGAPI2_NODEINTERFACE_INTEGER, BGAPI2_NODEINTERFACE_FLOAT
#define BGAPI2_NODEREPRESENTATION_PURENUMBER "PureNumber"   //valid for BGAPI2_NODEINTERFACE_INTEGER, BGAPI2_NODEINTERFACE_FLOAT
#define BGAPI2_NODEREPRESENTATION_BOOLEAN "Boolean"         //valid for BGAPI2_NODEINTERFACE_INTEGER
#define BGAPI2_NODEREPRESENTATION_HEXNUMBER "HexNumber"     //valid for BGAPI2_NODEINTERFACE_INTEGER
#define BGAPI2_NODEREPRESENTATION_IPV4ADDRESS "IPV4Address" //valid for BGAPI2_NODEINTERFACE_INTEGER
#define BGAPI2_NODEREPRESENTATION_MACADDRESS "MACAddress"   //valid for BGAPI2_NODEINTERFACE_INTEGER

#endif //__BGAPI2_DEF_H__
