#include "stdafx.h"
#include "cMyTimer.h"
#include "windows.h"
#include "process.h"

cMyTimer::cMyTimer()
{
	
}

cMyTimer::~cMyTimer()
{
	ClearTimer();
}

void cMyTimer::ClearTimer()
{
	for (itTimerList it = m_timerList.begin();it != m_timerList.end();++it)
	{		
		it->bDel = true;
	}
}


void CheckTimerRun(void* p)
{
	cMyTimer* pTimer = (cMyTimer*)p;
	if (pTimer == NULL)
	{
		return;
	}
	while(1)
	{
		pTimer->CheckDelTimer();
		
		//检测是否有定时器要运行
		if(!pTimer->TimerRun())
		{
			_endthread();
		}
		Sleep(20);
	}
}

//添加定时器
void cMyTimer::AddTimer(unsigned timerId,unsigned timeMs,int param,char* p)
{
	if (timeMs == 0)
	{
		return;
	}
	stTimer stTimerTemp;
	stTimerTemp.bDel = false;
	stTimerTemp.id = timerId;
	stTimerTemp.iParam = param;
	if (p != NULL)
	{
		stTimerTemp.strParam = p;
	}
	
	unsigned timeNow = GetTickCount();
	stTimerTemp.timeCreate = timeNow;
	stTimerTemp.timeLastRun = timeNow;
	stTimerTemp.timeElapce = timeMs;
	m_timerList.push_back(stTimerTemp);
	if (m_timerList.size() == 1)
	{
		//说明此时是第一个定时器， 需要开启线程
		_beginthread(CheckTimerRun,0,this);
	}
}

//删除定时器
void cMyTimer::DeleteTimer(int id)
{
	for (itTimerList it = m_timerList.begin();it != m_timerList.end();++it)
	{
		if (it->id == id)
		{
			it->bDel = true;
			return;
		}
	}
}




//定时器处理
//int cMyTimer::OnTimer(int id,int iParam,string str)
//{
//	return 1;
//}


//定时检测删除定时器
void cMyTimer::CheckDelTimer()
{
	for (itTimerList it = m_timerList.begin();it != m_timerList.end();)
	{
		if (it->bDel)
		{
			m_timerList.erase(it);
			it = m_timerList.begin();
			continue;
		}
		++it;
	}
}

bool cMyTimer::TimerRun()
{
	if (m_timerList.size() == 0)
	{
		return false;
	}
	unsigned timeNow = GetTickCount();
	for (itTimerList it = m_timerList.begin();it != m_timerList.end();++it)
	{
		if (timeNow-it->timeLastRun >= it->timeElapce)  //判断时间间隔
		{
			it->timeLastRun = timeNow;
			if(OnTimer(it->id,it->iParam,it->strParam) == 0)
			{
				//删除定时器
				it->bDel = true;
			}
			continue;
		}
	}
	return true;
}