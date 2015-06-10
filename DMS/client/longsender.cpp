#include"longsender.h"
#include"dmsexception.h"
#include<iostream>
#include<stdio.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
using namespace std;

	LogSender::LogSender(){
		port=8899;
		strcpy(severIp,"127.0.0.1");
		strcpy(sendfailFileName,"sendfail.dat");
	}


	LogSender::~LogSender(){}
	
	bool LogSender:: sendMatche(MatchedLogRec mlog){
		int sfd=send(fd,&mlog,sizeof mlog,0);
		if(sfd>0){
			return true;
		}
		return false;
	}


	void LogSender::closefd(){
		close(fd);//因为fd是私有的，所以为了在类外界访问而引入了该公开的closefd接口函数
	}
	void LogSender::sendMatches(list<MatchedLogRec>* matches){
		try{
		initNetWork();
		readSendFailed(matches);
		/*若初始化成功，则循环的发送数据*/
		while(matches->size()>0){
			int sfd=send(fd,&(*(matches->begin())),sizeof(MatchedLogRec),0);
			if(sfd<=0){
				throw DmsSendDataException("send data to sever failed");
			}
			matches->erase(matches->begin());
		}
		}catch(DmsInitNetWorkException &e){
			cout<<e.what()<<endl;
		}catch(DmsSendDataException &e){
			cout<<e.what()<<endl;
		}
		close(fd);//关闭客户端
	}
	//在循环里抛出异常，是否会退出循环？答：会，一抛出异常，循环就会自动终断
	void LogSender::initNetWork(){
		fd=socket(AF_INET,SOCK_STREAM,0);
		if(fd==-1){
			throw DmsInitNetWorkException("init socket failed");
		}
		//连接服务器
		struct sockaddr_in addr={0};
		addr.sin_family=AF_INET;
		addr.sin_port=htons(port);
		addr.sin_addr.s_addr=inet_addr(severIp);
		int cfd=connect(fd,(sockaddr*)&addr,sizeof addr);
		if(cfd==-1){
			throw DmsInitNetWorkException("connect server failed");
		}//注：如果在捕获异常时，想要用catch(DmsInitNetWorkException *e){
		 /*          cout<<e->what()<<endl;的形式，就必须要将 throw DmsInitNetWorkException("connect server failed");改做 throw  new DmsInitNetWorkException("connect server failed");表示new一个异常
*/
	}


	void LogSender::readSendFailed(list<MatchedLogRec>* matches){}
	void LogSender::saveSendFailed(list<MatchedLogRec>* matches){}

//注：在Vi编辑器中，若要将某个字符替换成其他内容，则可以在命令行模式下输入下述命令：
//%s /;/{}然后按回车
