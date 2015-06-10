#include<iostream>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<pthread.h>//编译时只需要这个头文件即可，但在链接时要加上-lpthread
#include "data.h"
#include <deque>//定义一个双端队列，可实现数据的先进先出
using namespace std;
/*在全局定义一个数据缓冲池，保证数据先进先出*/
//标准库里的队列线程并不安全，则需要自己来控制安全性
deque<MatchedLogRec> datapool;
class ProducterThread{
/*线程处理 函数*/
static void* reciveData(void* par){
	((ProducterThread*)par)->run();
}
public:
void run(){
//	int afd=*((int*)par);
	while(1){
	MatchedLogRec mlog={0};
	int rfd=recv(afd,&mlog,sizeof mlog,0);
	if(rfd>0){
		datapool.push_back(mlog);//缓冲池中存入数据
		cout<<afd<<":"<<mlog.logname<<":"<<mlog.logip<<endl;
	}else{
		break;
	}
	}
	close(afd);
	delete this;
}
//生产者线程
	pthread_t thid;
	int afd;//afd为客户端文件描述符
	public:
	ProducterThread(int afd){
		this->afd=afd;
	}
	//创建线程
	public:
	void start(){//普通成员函数
		pthread_create(&thid,0,reciveData,this);
	}
};
/*写一个消费者线程,负责从池中取出数据*/
class CustomerThread{
private:
	pthread_t thid;
public:
	/*线程处理函数*/
	static void* getData(void* par){
	
	}
	/*真正完成功能的函数*/
	public:
	void run(){
	
	}
	/*启动线程*/
	public:
	void start(){
	
	}
};
int main(){
	int fd=socket(AF_INET,SOCK_STREAM,0);
	if(fd==-1){
		cout<<"init server failed"<<endl;
	}
	struct sockaddr_in addr={0};
	addr.sin_family=AF_INET;
	addr.sin_port=htons(8899);
	addr.sin_addr.s_addr=INADDR_ANY;
	int bfd=bind(fd,(struct sockaddr*)&addr,sizeof addr);
	if(bfd==-1){
		cout<<"server bind failed"<<endl;
	}
	while(1){
	listen(fd,10);
	struct sockaddr_in caddr={0};
	socklen_t slen=sizeof caddr;
	int afd=accept(fd,(struct sockaddr*)&caddr,&slen);
	if(afd==-1){
		cout<<"accept client failed"<<endl;	
		break;//如果服务端接收失败，就终止服务器
	}
/*在这里创建线程，把afd传给线程*/
/*	pthread_t thid;
	pthread_create(&thid,0,reciveData,&afd);*/
	/*把线程包装成类*///(因为将线程已经封装成了类，所以可将上面注释了，写成下面的形式)
	ProducterThread *producter=new ProducterThread(afd);
	producter->start();
	/*
	//保证从一个客户端不断接收数据，加此循环
	while(1){
	MatchedLogRec mlog={0};
	int rfd=recv(afd,&mlog,sizeof mlog,0);
	if(rfd>0){
		cout<<afd<<":"<<mlog.logname<<":"<<mlog.logip<<endl;
	}else{
		break;
	}
	}
	close(afd);*/
	}//为使其中一个客户端关闭而，服务器仍然继续运行，加此循环
	close(fd);
	/*把线程包装成类*/

}
//g++ dmsserver.cpp -lpthread
