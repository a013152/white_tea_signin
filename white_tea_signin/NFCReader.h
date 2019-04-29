#pragma once
#include "stdafx.h"
#include <windows.h>
#include "./include/hfrdapi.h"
#pragma comment(lib,"./lib/hfrdapi.lib") 

#include "cMyTimer.h"
#include <string>
using namespace std;

enum TimerType{
	Timer_Read_NFC = 1,
};

//NFC读取类
#define GET_NFC CNFCReader::getInstance()
class CNFCReader:public cMyTimer
{
	CNFCReader();
	HINSTANCE m_hInstMaster;
	CALLBACK_FUN m_pFun;
public:	
	/*获取类实例*/
	static CNFCReader* getInstance();
	static void  freeInstance();
	~CNFCReader(); 

	bool openDev();
	string readNfc();
	void setCallbackFun(CALLBACK_FUN fun){ m_pFun = fun; }

	virtual int OnTimer(int id, int iParam = 0, string str = "");
};

