#ifndef DMSCLIENT_H
#define DMSCLIENT_H
#include<QDialog>
#include<QPushButton>
#include<QTextBrowser>
#include"mythread.h"//在此定义一个与界面绑定的线程而引入
class DmsClient:public QDialog{
	Q_OBJECT
private:
	QTextBrowser *showdata;
	QPushButton *bstart;
	QPushButton *bclose;
	/*和界面绑定的线程指针*/
	MyThread *myth;
public:
	DmsClient();
	~DmsClient();
	public slots:
	void startClient();
	/*接收线程的信号数据*/
	public slots:
	void getData(QString data);
};
#endif
