#ifndef __ERROR_CODE_H
#define __ERROR_CODE_H

#pragma once

/** two level error,the range between (0,0x1ff) is common error,it can recover anther is system level error and can't be recover */
enum ErrorCodeEnum
{
	Error_Succeed=0,
	
	//date check category
	Error_DataCheck=100,		//mark the beginning for this category
	Error_Null,					//the error of a null pointer
	Error_Param,				//the error of a illegal parameter
	Error_Overflow,				//The length or range overflow
	Error_TooSmallBuffer,		// buffer too small
	Error_NotIntegrated,		//The package or result is not integrated

	//Target being category
	Error_TargetBeing=0x200,		
	Error_NoTarget,				//request target is not exist
	Error_NoDefine,				//The class or object no define
	Error_NotImpl,				//the method has not implement yet
	Error_NotExist,				//not exist
	Error_Duplication,			//Duplicate action or object
	Error_Unregisted,			//exist but the object has not registered
	Error_AlreadyExist,			//already exist
	Error_MethodNotFound,		//method does not exist
	Error_Redirect,				// redirect indication

	//Entity state category
	Error_InvalidState=0x300,	//state is not valid for current call
	Error_NotInit,				//The target is not be initial
	Error_Paused,				//the access object is in Pause state
	Error_Stoped,				//the access object is in Stop state
	Error_Losted,				//the access object is in Lost state 
	Error_Closed,				//opposite side in close state

	//Framework Task control category
	Error_TaskControl=0x400,
	Error_Pending,				//The request in not finish
	Error_Cancel,				//The process is be cancel by framework
	Error_Break,				//The task has interrupt
	Error_NotMeetCondition,		//not meat run condition,run cancel
	Error_NoPrivilege,			//no privilege
	Error_MethodSignatureFailed,//method signature failed

	//opposite side action category 
	Error_PeerAction=0x500,
	Error_PeerClose,			//peer close request
	Error_PeerIgnore,			//peer did not answer before the ITransactionContext object release
	Error_PeerReject,			//The another side reject the request
	Error_PeerDelay,			//The task can not run right now,will be delayed to set time

	//process fail category
	Error_Process=0x600,
	Error_NetBroken,			//the network is broken
	Error_UpdateFailed,			//system update failed
	Error_RegistryFailed,		//registry operation failed
	Error_IO,					//IO error(file reading/writing)
	Error_Readonly,				//The object can't be edit

	Error_TimeOut=0x700,
	Error_BlockTimeOut,			//WaitAnswer time out
	Error_ThreadTimeOut,			//Max Thread run time
	Error_QueueTimeOut,			//Task wait time out in queue
	Error_ReplyTimeOut,			//The system max answer time

	Error_Hardware=0x800,
	Error_DevLoadFileFailed,	//load dll or config file failed
	Error_DevNotAvailable,		//device not connected
	Error_DevAlreadyConnected,	//device already connected
	Error_DevConnFailed,		//connect to device failed
	Error_DevCommFailed,		//Communication failed between HOST and Device
	Error_DevMedia,				//Media error(Data lack,unrecognized and so on)

	Error_Debug=0xf00,
	Error_Assert,
	Error_Trace,
	Error_Bug,					//bug detected
	
	/*
	// device error code x define at range 0x00010000 <= x <= 0x0001ffff
	// 
	Error_Device_Begin = 0x00010000, 
	//...... device error codes goes here
	Error_Device_End = 0x0001ffff,
	*/

	Error_Unrecover=0x70000000,
	Error_Resource,				//The system resource ec:memory/handle/space,do not retry,should restart system
	Error_NewProcess,			//start process failed
	Error_FailVerify,			//Signature verification failed of entity
	Error_Block,				//The thread block and the module hold
	Error_Exception,			//except throw
	Error_Unexpect=0x7ffffffe,	//the fail reason is not expect

	Error_IgnoreAll=0x7fffffff // for special internal usage only, user dont use this code
};

#define ERR_(e) #e

#endif // __ERROR_CODE_H