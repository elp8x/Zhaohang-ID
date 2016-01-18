#ifndef __CARD_MIX_H
#define __CARD_MIX_H
#pragma once

const int MAX_MAG_TRACK_NUM = 3;
const int MAX_MAG_TRACK_SIZE = 512;
const int MAX_IC_BUFFER_SIZE = 1024;


//move card to position
enum CardPosEnum
{
	CI_MOVECARD_FRONT_GATE,		//reject to front gate,hold card
	CI_MOVECARD_RF_POSITION,	//rf position
	CI_MOVECARD_IC_POSITION,	//ic position
	CI_MOVECARD_MAG_POSITION,	//magnetic position
	CI_MOVECARD_BACK_NOT_HOLD,	//capture to retain bin
	CI_MOVECARD_FROM_HOPPER,	//card set from hopper
	CI_MOVECARD_RETRIEVE,		//retrieve card,which is in Gate position
};
//card entry type
enum CardInEnum
{
	CI_CARD_IN_TYPE_FORBIDDEN,	//forbidden card entry
	CI_CARD_IN_TYPE_MAG,		//accept only Magnetic card entry
	CI_CARD_IN_TYPE_ALL			//accept any card
};
enum CardStatusEnum
{
	CI_MEDIA_NOTPRESENT,	//no card in machine
	CI_MEDIA_PRESENT,		//detect card in machine
	CI_MEDIA_JAMMED,		//card jammed
	CI_MEDIA_ENTERING,		//card on front gate
	CI_MEDIA_IC,		//detect IC card in machine
	CI_MEDIA_NOT_IC,	//detect card in ic slot,but can't active,maybe not ic card
	CI_MEDIA_RF,		//detect RF card
	CI_MEDIA_IDCARD,	//ID Certificate Card
};
//retain bin state
enum RtBinStatusEnum
{
	CI_RETAINBIN_OK,	//number of retained card less than half of retain bin's capacity
	CI_RETAIN_NOTSUPP,	//no retain bin
	CI_RETAINBIN_FULL,	//retain bin is full
	CI_RETAINBIN_HIGH,	//retain bin is almost full,number of retain card more 
	//than half of retain bin's capacity
};
//issue hopper state
enum IssHopperStatusEnum
{
	CI_ISSUEHOPPER_OK,	//number of issued card more than half of issue hopper's capacity
	CI_ISSUEHOPPER_NOTSUPP,	//no issue hopper
	CI_ISSUEHOPPER_EMPTY,	//issue hopper is empty
	CI_ISSUEHOPPER_LOW,		//issue hopper is almost empty,number of card less 
	//than half of issue hopper's capacity
};
enum LightPosEnum
{
	CI_LIGHT_MAG = 0x1,			//the magnetic stripe light flicker
	CI_LIGHT_IC = 0x2,			//the IC card light flicker
	//CI_LIGHT_MAG_IC,		//both the magnetic and IC light flicker
	CI_LIGHT_CONTACTLESS = 0x4,	//the contactless card light flicker
	CI_LIGHT_IDCERTIFICATE = 0x8,	//the IDCertificate card light flicker
};
enum TrackSrcEnum
{
	CI_TRACK_SOURCE_1,	//magnetic track 1
	CI_TRACK_SOURCE_2,	//magnetic track 2
	CI_TRACK_SOURCE_3,	//magnetic track 3
};
//magnetic track range for read&write
enum TrackRange
{
	CI_TRACK_RANGE_START = 0,
	CI_TRACK_RANGE_1 = 1,	//track1
	CI_TRACK_RANGE_2,		//track2
	CI_TRACK_RANGE_1_2,		//track1&track2
	CI_TRACK_RANGE_3,		//track3
	CI_TRACK_RANGE_1_3,		//track1&track3
	CI_TRACK_RANGE_2_3,		//track2&track3
	CI_TRACK_RANGE_1_2_3,	//track1&track2&track3
};
//track data state
enum TrackDataStateEnum
{
	CI_DATA_OK,		//data ok
	CI_DATA_INVALID,
};
enum MagWriteModeEnum
{
	HIGH_CO,
	LOW_CO,
	AUTO_CO
	//HIGH_CO_NORMAL,	//high-co card writing with normal read method at write-verify
	//LOW_CO_NORMAL,	//low-co card writing with normal read method at write-verify
	//AUTO_CO_NORMAL,	//auto judge high-co/low-co card with normal read method at write-verify
	//HIGH_CO_EXTRA,	//high-co card writing with extra read method at write-verify
	//LOW_CO_EXTRA,	//low-co card writing with extra read method at write-verify
	//AUTO_CO_EXTRA	//auto judge high-co/low-co card with extra read method at write-verify
};
enum MifareFuctionEnum
{
	CI_MIFARE_LOAD_KEY,	//load key
	CI_MIFARE_AUTH,		//authentication
	CI_MIFARE_READ,		//read
	CI_MIFARE_WRITE,	//write
	CI_MIFARE_INC,		//increment transfer
	CI_MIFARE_DEC		//decrement transfer
};

struct CmdInfo
{
	DWORD dwSize;
	BYTE data[MAX_IC_BUFFER_SIZE];
};

struct TrackInfo
{
	TrackSrcEnum eSource;
	TrackDataStateEnum eStatus;
	DWORD dwSize;
	BYTE data[MAX_MAG_TRACK_SIZE];
};
struct MagTracks
{
	TrackRange eRange;	
	TrackInfo track[MAX_MAG_TRACK_NUM];
};
#endif//__CARD_MIX_H