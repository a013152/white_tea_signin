// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <time.h>
#include <windows.h>
#include <vector>
#include <regex>
using namespace std;


#define MAX8192 8192
#define MAX1024 1024


std::string getAppDir();
extern std::string g_strAppDir;   //Ŀ¼
extern std::string g_strAppSetIniPath;  //�����ļ�·��
void initSys();
std::string UTF_82ASCII(std::string& strUtf8Code);
std::string ASCII2UTF_8(std::string& strASCIICode);
std::string  G2U(const char* gb2312);
typedef void(*CALLBACK_FUN)(const char*);

#define  TIME_INTERVEAL 200

enum TimerType{
	Timer_Read_NFC = 1,  //��ȡNFC
	Timer_Auto_login,    //�Զ���½
};


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
