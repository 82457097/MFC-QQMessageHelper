#pragma once
class QQMessage {
public:
	QQMessage();
	~QQMessage();
	//为这个类写一个入口
	static QQMessage *Instance() {
		static QQMessage object;	//生成对象
		return &object;
	}
	//为功能文件开始函数
	void Start(const wchar_t *msg);

private:
	//开启线程
	static unsigned int WINAPI runThreadProc(void *parm);
	//发送的流程
	void sendAll();
	//模拟按键
	void pressKey(BYTE vkey);

	//发送消息
	void sendMsg();

	//需要把消息拷贝到剪贴板
	void addToClipBoard();

	CString m_msg;

};

