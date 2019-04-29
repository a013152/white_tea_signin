// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <time.h>
#include <windows.h>
#include <vector>

using namespace std;


#define MAX8192 8192
#define MAX1024 1024


std::string getAppDir();
extern std::string g_strAppDir;   //目录
extern std::string g_strAppSetIniPath;  //配置文件路径
void initSys();
std::string UTF_82ASCII(std::string& strUtf8Code);

typedef void(*CALLBACK_FUN)(const char*);

// TODO:  在此处引用程序需要的其他头文件
