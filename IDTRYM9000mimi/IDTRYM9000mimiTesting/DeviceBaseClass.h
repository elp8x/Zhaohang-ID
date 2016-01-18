/////////////////////////////////////////////////////////////////////////////////
///	Copyright (c) 2012 China Merchants Bank, all rights reserved	       	  ///
///										                         	       	  ///
///	Base defination for device adapter.                                                             	       	  ///
/// DeviceBaseClass.h: Interface for the DeviceBaseClass class.				  ///
///																			  ///
/////////////////////////////////////////////////////////////////////////////////

//	Return code(ErrorCodeEnum):
//	Error code that adapters returned mainly in the following list:
//	Error_Hardware=0x800,
//	Error_DevLoadFileFailed,	//load dll or config file failed
//	Error_DevNotAvailable,		//device not connected
//	Error_DevAlreadyConnected,	//device already connected
//	Error_DevConnFailed,		//connect to device failed
//	Error_DevCommFailed,		//Communication failed between HOST and Device
//	Error_DevMedia,				//Media error(Data lack,unrecognized and so on)
//	Adapters can also return error code which included in the file ErrorCode.h

//	The "Optional." in front of method's comment means this method to implement depends on device.
//	If the device don't have the function ,just return Error_Succeed simply,but must declare in the 
//	implementation document. 

#ifndef __DEVICE_BASE_CLASS_H
#define __DEVICE_BASE_CLASS_H

#include "ErrorCode.h"

#ifdef __cplusplus
extern "C"
	{
#endif

const int MAX_DEV_TYPE_LEN = 256;
const int MAX_DEV_MODEL_LEN = 32;
const int MAX_DEV_VENDOR_LEN = 256;
const int MAX_DEV_ERROR_MSG_LEN = 256;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

//version info of device software
struct DevSoftVersion
{
	WORD wMajor;		//release major version
	WORD wMinor;		//release minor version
	WORD wRevision;		//bug repair version with the major and minor version remains the same
	WORD wBuild;		//compile version
};
enum DevStateEnum
{
	DEVICE_STATUS_NOT_READY,	//uninit
	DEVICE_STATUS_NORMAL,		//normal
	DEVICE_STATUS_MAINTAINCE,	//need to maintaince
								//ex:paper tray is empty,retain bin is full
	DEVICE_STATUS_FAULT,		//cannot work
};
struct DevCategoryInfo
{
	char szType[MAX_DEV_TYPE_LEN];	//device type sth like "CMB.Printer.HP1234"
	char szModel[MAX_DEV_MODEL_LEN];	//device model
	char szVendor[MAX_DEV_VENDOR_LEN];		//device vendor
	DevStateEnum eState;		//device status
	DevSoftVersion version;	//software version
};
struct DevErrorInfo
{
	DWORD dwErrMsgLen;
	char szErrMsg[MAX_DEV_ERROR_MSG_LEN];
};
class DeviceBaseClass  
{
public:
	//	
	//	Get category infomation about device.
	//
	virtual ErrorCodeEnum GetDevCategory(DevCategoryInfo &devCategory) = 0;
	//	Reset device.
	//	Do the cleaning work and initialize device again in order to return to
	//	the normal condition.
	virtual ErrorCodeEnum Reset() = 0;
	//	
	//	Close device and do the cleaning work.
	//	ex. close connection,close port,release memery and so on
	virtual ErrorCodeEnum DevClose() = 0;
	//	Get last error the device issued.
	//	Error message must include explanatory memorandum ,the original error 
	//	code and anything in favour of location problem.
	virtual ErrorCodeEnum GetLastErr(DevErrorInfo &devErrInfo) = 0;
};

#ifdef DEVICEBASE_EXPORTS
#define DEVICEBASE_API __declspec(dllexport)
#else
#define DEVICEBASE_API __declspec(dllimport)
#endif

DEVICEBASE_API ErrorCodeEnum  CreateDevComponent(DeviceBaseClass *&baseObj);
DEVICEBASE_API ErrorCodeEnum  ReleaseDevComponent(DeviceBaseClass *&pBaseObj);
#ifdef __cplusplus
}
#endif

#endif // __DEVICE_BASE_CLASS_H
