#pragma once
#include "stdafx.h"
#include <windows.h>
#include "./include/hfrdapi.h"
#pragma comment(lib,"./lib/hfrdapi.lib") 

#include "cMyTimer.h"
#include <string>
using namespace std;



//NFC读取类
#define GET_NFC CNFCReader::getInstance()
class CNFCReader:public cMyTimer
{
	CNFCReader();
	HINSTANCE m_hInstMaster;
	CALLBACK_FUN m_pFun;

	int tempInterval;

	string strPattern = "[\\s\\S]*shop.tdneed.com[\\s\\S]*";
	string strConPattern = "[\\s\\S]*[A-Z]-[0-9]{5}-[0-9][\\s\\S]*";
	string strURLPattern2 = "shop.tdneed.com[\\s\\S]*";
	string strConPattern2 = "[A-Z]-[0-9]{5}-[0-9][\\s\\S]*";

public:	
	/*获取类实例*/
	static CNFCReader* getInstance();

	static void  freeInstance();

	~CNFCReader(); 

	bool openDev();

	string readNfc();

	void setCallbackFun(CALLBACK_FUN fun){ m_pFun = fun; }

	virtual int OnTimer(int id, int iParam = 0, string str = "");

	void doAnalyzeNFC(const string str);
};

