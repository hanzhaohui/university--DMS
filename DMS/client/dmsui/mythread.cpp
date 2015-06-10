#include"mythread.h"
#include "../longreader.h"//使用绝对路径找到该头文件
#include "../longsender.h"
#include<stdio.h>
void MyThread::run(){
/*在这里实现线程的功能*/
	/*通知界面发送了哪些数据*/
	LogReader logreader;
	LogSender logsender;
	list<MatchedLogRec> matches=logreader.readLogs();//创建一个接收集合
//如果发送一个集合，不能把具体的数据通知界面
//	logsender.sendMatches(&matches);//发送数据（因为发送的是个集合，所以只实现了发送数据，而并没有通知界面）
	/*能拿到每次发送的数据（不能，因为数据是装到集合中发送出去的）*/
	/*如何拿到每次发送的数据，使用循环发送数据,拿到集合中的数据*/
	logsender.initNetWork();//先连接上网络
	while(matches.size()>0){
	//	logsender.sendMatche(一条具体的数据);
		sleep(1);
		MatchedLogRec mlog=*(matches.begin());
		bool f=logsender.sendMatche(mlog);//发送一条数据
		if(f){//表示发送成功
		/*把发送的数据结构体组织成字符串 通知界面*/
			char datastr[100];
			sprintf(datastr,"%s:%d:%d",mlog.logname,mlog.pid,mlog.logintime);
			/*通知界面*/
			emit mySig(QString(datastr));//将字符串转换为QString类型
			matches.erase(matches.begin());//删除发送成功的第一条数据
		}else{
			break;
		}
	}
	logsender.closefd();//调用成员函数来关闭fd，否则服务器无法识
}
