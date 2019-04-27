#pragma once
#include "string"
#include "list"
using namespace std;

struct stTimer
{
	unsigned timeElapce;	//定时器间隔运行时间
	unsigned timeCreate;	//定时器创建时间
	unsigned timeLastRun;	//定时器上次执行时间
	unsigned id;					//定时器ID号
	int iParam;				//预留参数
	string	strParam;		//预留参数
	bool bDel;				//是否已被删除

	stTimer()
	{
		timeCreate = 0;
		timeLastRun = 0;
		id = -1;
		iParam = 0;
		bDel = false;
	}
};

typedef list<stTimer> stTimerList;
typedef list<stTimer>::iterator itTimerList;

class cMyTimer
{
public:
	cMyTimer();
	virtual ~cMyTimer();

	//添加定时器
	void AddTimer(unsigned timerId, unsigned timeMs,int param = 0,
		char* p=NULL);
	//删除定时器
	void DeleteTimer(int id);

	//定时器处理
	virtual int OnTimer(int id,int iParam,string str) = 0;
	
	//检测定时器运行
	bool TimerRun();
	
	//清除所有定时器
	void ClearTimer();
	//定时检测删除定时器
	void CheckDelTimer();	
private:
		
	

	stTimerList m_timerList;	//定时器列表
};