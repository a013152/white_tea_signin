#pragma once
#include <windows.h>
#include "./include/hfrdapi.h"
#pragma comment(lib,"./lib/hfrdapi.lib") 

#include "cMyTimer.h"
#include <string>
using namespace std;

enum TimerType{
	Timer_Read_NFC = 1,
};

//NFC��ȡ��
#define GET_NFC CNFCReader::getInstance()
class CNFCReader:public cMyTimer
{
	CNFCReader();
	HINSTANCE m_hInstMaster;
public:	
	/*��ȡ��ʵ��*/
	static CNFCReader* getInstance();
	static void  freeInstance();
	~CNFCReader(); 


	bool openDev();
	string readNfc();

	virtual int OnTimer(int id, int iParam = 0, string str = "");
};

