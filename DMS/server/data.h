#ifndef DATA_H
#define DATA_H
//这是登录登出对应的记录类型
 struct LogRec{
 	char logname[32];
	int pid;
	short logtype;
	int logtime;
	char logip[257];
 };
//匹配记录对应的类型(此数据类型最终是存在服务器上的)
  struct MatchedLogRec{
  	char logname[32];
	int pid;
	int logintime;
	int logouttime;//对应到服务器上是（日期类型）
   int durations;//时间差，对应到服务器上是（number类型）
	char logip[257];//客户端的ip
	/*服务器的ip*/
	char labip[20];//若想统计每台服务器时，就需加入这一个字段
  };
#endif

