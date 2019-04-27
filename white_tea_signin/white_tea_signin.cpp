// white_tea_signin.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include "NFCReader.h" 

int _tmain(int argc, _TCHAR* argv[])
{
	if (GET_NFC->openDev()){
		//GET_NFC->readNfc();
		
	}
	getchar();
	GET_NFC->freeInstance();
	return 0;
}

