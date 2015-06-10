#include"mythread.h"
void MyThread::run(){
/*在这里实现线程的功能*/
	qDebug("sned data to server");
	/*通知界面发送了那些数据*/
	emit mySig("send data to server");//发送一条数据就发射一个信号
	sleep(1);
	qDebug("send data to server" );
	emit mySig("send data to server");
	sleep(1);
	qDebug("send data to server" );
	emit mySig("send data to server");
	sleep(1);
	
}
