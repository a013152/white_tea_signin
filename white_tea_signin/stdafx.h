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

std::string getAppDir();
extern std::string g_strAppDir;   //Ŀ¼
extern std::string g_strAppSetIniPath;  //�����ļ�·��
void initSys();


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
