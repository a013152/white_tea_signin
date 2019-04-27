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
		
		//����Ƿ��ж�ʱ��Ҫ����
		if(!pTimer->TimerRun())
		{
			_endthread();
		}
		Sleep(20);
	}
}

//��Ӷ�ʱ��
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
		//˵����ʱ�ǵ�һ����ʱ���� ��Ҫ�����߳�
		_beginthread(CheckTimerRun,0,this);
	}
}

//ɾ����ʱ��
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




//��ʱ������
//int cMyTimer::OnTimer(int id,int iParam,string str)
//{
//	return 1;
//}


//��ʱ���ɾ����ʱ��
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
		if (timeNow-it->timeLastRun >= it->timeElapce)  //�ж�ʱ����
		{
			it->timeLastRun = timeNow;
			if(OnTimer(it->id,it->iParam,it->strParam) == 0)
			{
				//ɾ����ʱ��
				it->bDel = true;
			}
			continue;
		}
	}
	return true;
}