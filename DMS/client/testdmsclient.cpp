#include "longreader.h"
//测试文件
#include"longsender.h"
int main()
{  
	LogReader logreader;//定义一个用于读取的对象
	list<MatchedLogRec> matches=logreader.readLogs();//创建一个
	LogSender logsender;
	logsender.sendMatches(&matches);
}

