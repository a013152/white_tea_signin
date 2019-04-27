#pragma once
#include "string"
#include "list"
using namespace std;

struct stTimer
{
	unsigned timeElapce;	//��ʱ���������ʱ��
	unsigned timeCreate;	//��ʱ������ʱ��
	unsigned timeLastRun;	//��ʱ���ϴ�ִ��ʱ��
	unsigned id;					//��ʱ��ID��
	int iParam;				//Ԥ������
	string	strParam;		//Ԥ������
	bool bDel;				//�Ƿ��ѱ�ɾ��

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

	//��Ӷ�ʱ��
	void AddTimer(unsigned timerId, unsigned timeMs,int param = 0,
		char* p=NULL);
	//ɾ����ʱ��
	void DeleteTimer(int id);

	//��ʱ������
	virtual int OnTimer(int id,int iParam,string str) = 0;
	
	//��ⶨʱ������
	bool TimerRun();
	
	//������ж�ʱ��
	void ClearTimer();
	//��ʱ���ɾ����ʱ��
	void CheckDelTimer();	
private:
		
	

	stTimerList m_timerList;	//��ʱ���б�
};