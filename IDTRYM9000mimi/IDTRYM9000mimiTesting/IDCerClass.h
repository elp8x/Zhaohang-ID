/////////////////////////////////////////////////////////////////////////////////
///	Copyright (c) 2012 China Merchants Bank, all rights reserved	       	  ///
///	  Adapter Interface for Identity Card reader.                  	       	  ///
///	                                                             	       	  ///
////////////////////////////////////////////////////////////////////////////////
#ifndef __IDCER_CLASS_H
#define __IDCER_CLASS_H

#pragma once

#include "DeviceBaseClass.h"

const int MAX_IDCER_SIMPLE_INFO_SIZE = 32;
const int MAX_IDCER_INFO_SIZE = 128;
const int MAX_IDCER_PHOTO_PATH = 260;


struct IDCerInfo
{
	struct Name{
		CHAR data[MAX_IDCER_INFO_SIZE];
		DWORD dwSize;}name;
	struct Sex{
		CHAR data[MAX_IDCER_SIMPLE_INFO_SIZE];
		DWORD dwSize;}sex;
	struct Nation{
		CHAR data[MAX_IDCER_SIMPLE_INFO_SIZE];
		DWORD dwSize;}nation;
	struct Birthday{//format:1986Äê10ÔÂ07ÈÕ
		CHAR data[MAX_IDCER_SIMPLE_INFO_SIZE];
		DWORD dwSize;}birthday;
	struct Address{
		CHAR data[MAX_IDCER_INFO_SIZE];
		DWORD dwSize;}address;
	struct Idno{
		CHAR data[MAX_IDCER_SIMPLE_INFO_SIZE];
		DWORD dwSize;}idno;
	struct Department{
		CHAR data[MAX_IDCER_INFO_SIZE];
		DWORD dwSize;}department;
	struct StartDate{//format:2010.08.09
		CHAR data[MAX_IDCER_SIMPLE_INFO_SIZE];
		DWORD dwSize;}startDate;
	struct EndDate{	//format:2020.08.09
		CHAR data[MAX_IDCER_SIMPLE_INFO_SIZE];
		DWORD dwSize;}endDate;
	struct NewAddress{
		CHAR data[MAX_IDCER_INFO_SIZE];
		DWORD dwSize;}newAddress;
	struct PhotoPath{
		CHAR data[MAX_IDCER_PHOTO_PATH];
		DWORD dwSize;}photoPath;
};

class IDCerClass : public DeviceBaseClass  
{
public:
	//
	//	Open device.
	//	if usb device,set dwPort=0
	//
	virtual ErrorCodeEnum DevOpen(DWORD dwPort) = 0;
	//
	//	Optional.
	//	Open or close RF. 
	//	Arguments:
	//	- bControl:true:open RF,false close RF
	//
	virtual ErrorCodeEnum IDCerRFControl(bool bControl) = 0;
	//
	//	Optional.
	//	Certificate authentication.
	//
	virtual ErrorCodeEnum IDCerAuthenticate() = 0;
	//
	//	Get Identity Card info.
	//
	virtual ErrorCodeEnum IDCerGetData(IDCerInfo &idCerInfo) = 0;
};

#endif // __IDCER_CLASS_H
