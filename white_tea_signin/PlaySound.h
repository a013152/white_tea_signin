#pragma once
#include "stdafx.h"
#include <vector>
#pragma comment(lib,"WinMM.Lib")


#define GET_PLAYS CPlaySound::getInstance()

//音频播放类
class CPlaySound
{
	CPlaySound();
	void readSoundPath();  //读取配置
	vector<string> vtSoundPath;  //文件路径
	vector<string> vtPlayTask; //播放任务
	void playLoop();  //循环检测播放任务
	bool playLoopFlag; //循环检测播放任务的标志


	HANDLE			m_hShutdownEvent;				//关闭线程事件
	HANDLE			m_hPlayEvent;					//读线程事件
	HANDLE			m_hEventArray[2];				//线程事件数组

public:
	enum soundType{
		begin,
		end,
		fail1_exception, //异常，请稍后再试
		fail2_invalid,  //不是有效的采茶证件
		fail3_connect,  //网络异常
		fail4_connect,  //连接服务器异常
		fail5_already_signed,  //已经签到
	};

	static CPlaySound*getInstance();
	static void  freeInstance();
	static void playLoop_(void* p);
	~CPlaySound();
	HANDLE getShutdownEvent(){ return m_hShutdownEvent; }
	void addPlay(soundType type_);  //播放音频文件
};

