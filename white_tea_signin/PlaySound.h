#pragma once
#include "stdafx.h"
#include <vector>
#pragma comment(lib,"WinMM.Lib")


#define GET_PLAYS CPlaySound::getInstance()

//��Ƶ������
class CPlaySound
{
	CPlaySound();
	void readSoundPath();  //��ȡ����
	vector<string> vtSoundPath;  //�ļ�·��
	vector<string> vtPlayTask; //��������
	void playLoop();  //ѭ����ⲥ������
	bool playLoopFlag; //ѭ����ⲥ������ı�־


	HANDLE			m_hShutdownEvent;				//�ر��߳��¼�
	HANDLE			m_hPlayEvent;					//���߳��¼�
	HANDLE			m_hEventArray[2];				//�߳��¼�����

public:
	enum soundType{
		begin,
		end,
		fail1_exception, //�쳣�����Ժ�����
		fail2_invalid,  //������Ч�Ĳɲ�֤��
		fail3_connect,  //�����쳣
		fail4_connect,  //���ӷ������쳣
		fail5_already_signed,  //�Ѿ�ǩ��
	};

	static CPlaySound*getInstance();
	static void  freeInstance();
	static void playLoop_(void* p);
	~CPlaySound();
	HANDLE getShutdownEvent(){ return m_hShutdownEvent; }
	void addPlay(soundType type_);  //������Ƶ�ļ�
};

