#ifndef LONGSENDER_H
#define LONGSENDER_H
#include "data.h"
#include<list>
using namespace std;
class LogSender{
public:
	LogSender();
	~LogSender();
	/*增加发送一条数据的函数*/
	bool sendMatche(MatchedLogRec mlog);
	/*提供关闭fd的函数*/
	void closefd();
	void sendMatches(list<MatchedLogRec>* matches);
	void initNetWork();
	void readSendFailed(list<MatchedLogRec>* matches);
	void saveSendFailed(list<MatchedLogRec>* matches);
private:
	char sendfailFileName[50];
	int fd;//文件描述符
	unsigned short port;//端口
	char severIp[20];//连接服务器的ip地址
};

#endif// LONGSENDER_H
