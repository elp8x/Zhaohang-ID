#pragma once
#include "stdafx.h"
#include "IDCard.h"


ErrorCodeEnum  CreateDevComponent(DeviceBaseClass *&baseObj){
	LOG_INFO("#########################20160118#######################################################");
	LOG_INFO("################################################################################");
	LOG_INFO("��ʼ���ú��� CreateDevComponent��");
	ErrorCodeEnum enumErrorCodeReturn=Error_Null;
	baseObj=new CIDCard;
	if (!baseObj)
	{
		LOG_ERROR("��������ʧ�ܣ�");
		LOG_INFO("�������ú���CreateDevComponent ����ֵError_Null");
		return enumErrorCodeReturn;
	}
	enumErrorCodeReturn=Error_Succeed;
	LOG_INFO("�������ú���CreateDevComponent ����ֵError_Succeed");
	return enumErrorCodeReturn;
}
ErrorCodeEnum  ReleaseDevComponent(DeviceBaseClass *&pBaseObj){
	LOG_INFO("################################################################################");
	LOG_INFO("################################################################################");
	LOG_INFO("��ʼ���ú��� ReleaseDevComponent��");
	if (pBaseObj)
	{
		delete pBaseObj;
		pBaseObj=NULL;
	}
	LOG_INFO("�������ú���ReleaseDevComponent ����ֵError_Succeed");
	return Error_Succeed;
}