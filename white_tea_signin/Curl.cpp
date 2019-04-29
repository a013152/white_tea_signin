#include "stdafx.h"
#include "Curl.h"

CCurl* pthis = nullptr;
CCurl::CCurl()
{
}


CCurl* CCurl::getInstance()
{
	if (pthis == nullptr){
		pthis = new CCurl();
	}
	return pthis;
}

CCurl::~CCurl()
{
}
