// white_tea_signin.cpp : �������̨Ӧ�ó������ڵ㡣
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

