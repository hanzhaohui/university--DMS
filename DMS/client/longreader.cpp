#include "longreader.h"
#include<stdio.h>//time,rename等函数的定义均需引入该头文件
#include<iostream>
#include<cstring>
#include<fcntl.h>//由于open而导入该头文件
#include<unistd.h>
#include<arpa/inet.h>//因为htons()函数而引入
#include<sys/types.h>
#include<sys/stat.h>//注意写作#include<cunistd>是有错误的，对于标准C的头文件才可以进行两种不同形式的转换书写，而此处，unistd.h不是标准C中的头文件
using namespace std;
LogReader::LogReader(){
	strcpy(logFileName,"/home/oracle/DMS/client/wtmpx");//给出绝对路径
	strcpy(backFileName,"wtmpx");
	strcpy(failLoginsFileName,"/home/oracle/DMS/client/failmatch.dat");//因为没有给出绝对路径，所以此匹配失败的日志文件会在程序执行后在主目录下生成，所以在crontab -e下要进行配置
}

LogReader::~LogReader(){}
//读取日志文件的总调度函数，见详细设计.doc文档
list<MatchedLogRec> LogReader::readLogs(){
	backup();
	readFailLogins();
	readBackupFile();
	cout<<"logins size:"<<logins.size()<<endl;
	cout<<"longouts size:"<<logouts.size()<<endl;
	matchLogRec();
	cout<<"-----------------------------------------"<<endl;
	cout<<"logins size:"<<logins.size()<<endl;
	cout<<"longouts size:"<<logouts.size()<<endl;
	cout<<"matches size:"<<matches.size()<<endl;
	saveFailLogins();
	rename(backFileName,"/home/oracle/DMS/client/wtmpx");//加入此句可以再生wtmpx
	return matches;
}
void LogReader::backup(){
	cout<<"1.备份并清空日志文件"<<endl;
	/*yyyymmddhh:mi:sswtmpx*/
	time_t tim=time(NULL);
	struct tm* mytm=localtime(&tim);//将获得的单位为秒的时间转化为本地时间
	sprintf(backFileName,"/home/oracle/DMS/client/%4d%02d%02d%02d:%02d:%02dwtmpx",mytm->tm_year+1900,mytm->tm_mon+1,mytm->tm_mday,mytm->tm_hour,mytm->tm_min,mytm->tm_sec);
	/*
	 *man time.h中可看到int tm_year Years since 1900,所以在其后面要加上1900，其余所加的数都是这样的原因
	*/
	rename(logFileName,backFileName);//将logFileName改名为backFileName
	

}//将变化的文件变成不变的


void LogReader::readFailLogins(){
	cout<<"2.读取上一次未匹配成功的登入记录"<<endl;
	/*以结构体为单位，读取文件，读取的数据放入登入集合
	 * */
	//直接无限来读
	int fd=open(failLoginsFileName,O_RDONLY);
	if(fd==-1){
		return;
	}
	while(1){
		LogRec log={0};
		int rfd=read(fd,&log,sizeof log);//对一个非类型的对象求大小，用sizeof运算符时可以省略括号
		if(rfd<=0){
			break;
		}
		logins.push_back(log);
	}
	close(fd);
}

//结合详细设计.doc文档来实现函数(以wtmpx作为读取的文件)
void LogReader::readBackupFile(){
	cout<<"3.读取备份文件"<<endl;
	/*读取wtmpx文件*/
   int fd=open(backFileName,O_RDONLY);
	//打开文件
	if(fd==-1){
		cout<<"open backFileName failed"<<endl;
	}
	//得到文件的状态，使用fstst
	//准备一个结构体
	struct stat myfs={0};
	fstat(fd,&myfs);
	cout<<"file size:"<<myfs.st_size<<endl;
   //定义变量loopsize来确定循环次数，从而读文件
	int loopsize=myfs.st_size/372;//因为文档中定义每条登录数据占372个字节，则用文档总字节大小除以372，可得到共有多少条数据
	for(int i=0;i<loopsize;++i){
		LogRec log={0};//将读到的数据放到结构体中
		read(fd,&log.logname,32);//字符串不用考虑字节序问题，因为字符串就一个字节
		lseek(fd,36,SEEK_CUR);//读取id,此36是从需求说明.doc文档中获得
		//读取pid
		read(fd,&log.pid,4);
		log.pid=ntohl(log.pid);//为使长整型整数避免字节序问题
		//读取2个字节,得到登录类型
		read(fd,&log.logtype,2);
		log.logtype=htons(log.logtype);//用主机转网络来实现字节序的反转
		lseek(fd,6,SEEK_CUR);
		read(fd,&log.logtime,4);
		log.logtime=htonl(log.logtime);//若将时间不进行反转，就会出现负数的时间
		lseek(fd,30,SEEK_CUR);
		read(fd,&log.logip,257);
		lseek(fd,1,SEEK_CUR);

		if(log.logname[0]!='.'){//过滤掉以.开头的登录名
		//cout<<log.logname<<":"<<log.logtype<<endl;
		/*把logtype是7的放入登入集合，把logtype为8的放入登出集合*/
		if(7==log.logtype){
			logins.push_back(log);
		}else if(8==log.logtype){
			logouts.push_back(log);
		}else{
			;//空语句，表示不满足上面两个条件的就不做任何操作
		}
		}
	}
	close(fd);

}//读取备份文件
void LogReader::matchLogRec(){
	cout<<"4.匹配日志记录，得到匹配集合"<<endl;
	/*logname pid logip相同就是匹配记录*/
	/*思路：
	 *1.从登出集合中循环的取出登出记录
	 2.在登入集合中循环查找，如果找到匹配集合，就构建匹配记录，赋值，放入匹配集合，删除登入记录，终断本次查找， 如果不匹配就继续查找，直到查找结束
	 3.最后清空登出 集合
	 * */
	list<LogRec>::iterator iit;//登入集合的迭代器
	list<LogRec>::iterator oit;//登出集合的迭代器
	for(oit=logouts.begin();oit!=logouts.end();oit++){
		iit=logins.begin();
		while(iit!=logins.end()){
			if(!strcmp(iit->logname,oit->logname)&& iit->pid==oit->pid &&
			!strcmp(iit->logip,oit->logip)){//&&逻辑与具有短路特性，而&不具备短路特性，但其确实也可以进行结果的判断，得到相同的逻辑效果
				MatchedLogRec mlog={0};//构建匹配集合
				//赋值
				strcpy(mlog.logname,iit->logname);
				mlog.pid=iit->pid;
				mlog.logintime=iit->logtime;
				mlog.logouttime=oit->logtime;
				mlog.durations=mlog.logouttime-mlog.logintime;
				strcpy(mlog.logip,oit->logip);
				//放入匹配集合
				matches.push_back(mlog);
				//删除登入记录
				logins.erase(iit);
				//终断本次查找
				break;
			}
			iit++;
		}//对于普通的迭代器集合，如果对迭代器进行了曾，删，改等操作后，就要重启迭代器(即重新获得迭代器)才可进行++操作，否则就会得到不确定的结果.
	}
	logouts.clear();//清空登出集合
}
void LogReader::saveFailLogins(){
	cout<<"5.保存没有匹配成功的登录记录"<<endl;
	/*以结构体为单位，向文件中写入数据，写入一条删除一条，直到
	 * 集合数据写完
	 * */
 	int fd=open(failLoginsFileName,O_RDWR|O_CREAT|O_TRUNC,0777);
	if(fd==-1){
		cout<<"open matchfail file failed"<<endl;
	}
	while(logins.size()>0){//若集合中的元素个数大于0
  		int wfd=write(fd,&(*matches.begin()),sizeof(LogRec));//因为对于c++来说，语言的类型必须规范，不能将迭代器当指针使用，所以先家*号，再取地址用以将迭代器类型转成指针类型
		if(wfd<=0){
		break;
		}
	logins.erase(logins.begin());
	}
	close(fd);
}



