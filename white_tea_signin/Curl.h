#pragma once
#include "./include/curl/curl.h"
#pragma comment(lib,"./lib/libcurl.lib")    

//��������ӿ�http�������������
class CCurl
{
	CCurl();
public:
	static CCurl* getInstance();
	
	~CCurl();
};

