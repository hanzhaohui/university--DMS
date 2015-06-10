#ifndef MYTHREAD_H
#define MYTHREAD_H
#include<QThread>
class MyThread:public QThread{
Q_OBJECT//因自定义了信号函数，所以要加这个宏
public:
	void run();//要实现线程功能，就覆盖run()函数
	/*自定义信号，用来通知界面发送了哪些数据*/
	public:signals:
	void mySig(QString data);
};
#endif
