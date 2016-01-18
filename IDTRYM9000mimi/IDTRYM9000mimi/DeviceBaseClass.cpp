#pragma once
#include "stdafx.h"
#include "IDCard.h"


ErrorCodeEnum  CreateDevComponent(DeviceBaseClass *&baseObj){
	LOG_INFO("#########################20160118#######################################################");
	LOG_INFO("################################################################################");
	LOG_INFO("开始调用函数 CreateDevComponent！");
	ErrorCodeEnum enumErrorCodeReturn=Error_Null;
	baseObj=new CIDCard;
	if (!baseObj)
	{
		LOG_ERROR("创建对象失败！");
		LOG_INFO("结束调用函数CreateDevComponent 返回值Error_Null");
		return enumErrorCodeReturn;
	}
	enumErrorCodeReturn=Error_Succeed;
	LOG_INFO("结束调用函数CreateDevComponent 返回值Error_Succeed");
	return enumErrorCodeReturn;
}
ErrorCodeEnum  ReleaseDevComponent(DeviceBaseClass *&pBaseObj){
	LOG_INFO("################################################################################");
	LOG_INFO("################################################################################");
	LOG_INFO("开始调用函数 ReleaseDevComponent！");
	if (pBaseObj)
	{
		delete pBaseObj;
		pBaseObj=NULL;
	}
	LOG_INFO("结束调用函数ReleaseDevComponent 返回值Error_Succeed");
	return Error_Succeed;
}