#include "stdafx.h"
#include "QQMessage.h"


QQMessage::QQMessage() { 
}

QQMessage::~QQMessage() { 
}

//为功能文件开始函数
void QQMessage::Start(const wchar_t *msg) {
	//保存需要发送的消息
	m_msg = CString(msg);
	//创建一个线程
	_beginthreadex(NULL, 0, QQMessage::runThreadProc, NULL, 0,NULL);
}

//开启线程 线程的入口
unsigned int QQMessage::runThreadProc(void *parm) {
	QQMessage::Instance()->sendAll();	//开启发型流程
	return 0;
}

//发送的流程
void QQMessage::sendAll() {
	addToClipBoard();
	int errorTime = 0;
	int flag = 0;

	while (errorTime <= 4) {
		//1.找到窗口 QQ群发器 YY群发器 挂机软件...
		HWND qqhWnd = FindWindow(L"TXGuiFoundation", L"QQ"); //通过工具Spy++找到QQ的窗口 并定义一个句柄
		//2.找到的QQ窗口是不是在桌面的最前面
		HWND front_hWnd = GetForegroundWindow();	//得到当前最前面的窗口

		if (qqhWnd != front_hWnd) {
			if (!SetForegroundWindow(qqhWnd)) {
				//如果不在最前 需要把QQ窗口放到最前面
				MessageBox(NULL, L"找不到QQ窗口！", NULL, NULL);
				break;
			}
		}
		if (errorTime == 0) {
			//模拟按键 keybd_event();
			pressKey(VK_TAB);
			pressKey(VK_TAB);
		}
		pressKey(VK_DOWN); 
		pressKey(VK_RETURN);

		HWND curFrontWnd = GetForegroundWindow();
		if (curFrontWnd == qqhWnd) {
			++errorTime;
		}
		errorTime = 0;
		if(flag)
			sendMsg();
		flag++;
	}
}

//模拟按键
void QQMessage::pressKey(BYTE vkey) {
	//模拟按键 keybd_event();
	keybd_event(vkey, 0, 0, 0);	//按下按键
	Sleep(100);
	keybd_event(vkey, 0, KEYEVENTF_KEYUP, 0);	//弹起按键
	Sleep(100);
}

//发送消息
void QQMessage::sendMsg() { //正在测试消息群发器，如果打扰请见谅！！！
	//按下Ctrl+v
	keybd_event(VK_CONTROL, 0, 0, 0);
	Sleep(100);
	keybd_event(0x56, 0, 0, 0);
	Sleep(100);

	//弹起ctrl+v
	keybd_event(0x56, 0, KEYEVENTF_KEYUP, 0);
	Sleep(100);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
	Sleep(100);

	//发送关闭
	pressKey(VK_RETURN);
	pressKey(VK_ESCAPE);
}

//需要把消息拷贝到剪贴板
void QQMessage::addToClipBoard() {
	//内存拷贝
	int memLenth = m_msg.GetLength() * sizeof(wchar_t) + 2;
	HANDLE hGlobalMemery = GlobalAlloc(GHND, memLenth);
	void *memAddr = GlobalLock(hGlobalMemery);
	ZeroMemory(memAddr, memLenth);
	memcpy(memAddr, (const void*)m_msg.GetBuffer(), memLenth);
	GlobalUnlock(hGlobalMemery);

	//剪贴板操作操作
	::OpenClipboard(NULL);
	::EmptyClipboard();
	::SetClipboardData(CF_UNICODETEXT,hGlobalMemery);
	::CloseClipboard();
}