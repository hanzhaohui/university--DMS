#include "dmsclient.h"
DmsClient::DmsClient(){
	this->resize(500,500);
	showdata=new QTextBrowser(this);
	bstart=new QPushButton("start",this);
	bclose=new QPushButton("close",this);
	showdata->resize(440,390);
	showdata->move(30,30);
	bstart->move(100,450);
	bclose->move(350,450);
	/*创建线程对象*/
	myth=new MyThread();
	/*绑定线程到界面*/
	connect(myth,SIGNAL(mySig(QString)),this,SLOT(getData(QString)));
	//连接按钮到槽函数
	connect(bstart,SIGNAL(clicked()),this,SLOT(startClient()));//当点击bstart按钮就在startClient()中启动线程
	//连接按钮close
	connect(bclose,SIGNAL(clicked()),this,SLOT(close()));
}
DmsClient::~DmsClient(){
	delete showdata;
	delete bstart;
	delete bclose;
}
/*【1】uc的线程用在了qt中
 *把耗时的操作交给线程
 *使用线程处理函数) */
void* processData(void* par){/*该函数的返回值在这没用，而是对于pthread_join函数（其接收参数为二级指针）来接收*/
	QTextBrowser *showdata=(QTextBrowser*)par;//将参数转换为showdata这个局部变量，从而使该全局函数可以访问类里的该QTextBrowser *类型的变量
	sleep(1);
	showdata->append("send data to server");
	sleep(1);
	showdata->append("send data to server");
	sleep(1);
	showdata->append("send data to server");
	sleep(1);
	showdata->append("send over");	
	return par;
}//因为该线程处理函数是定义的全局函数，要想让其访问DmsClient类里的showdata成员变量，要写明作用域或者用上面所写的方法（只有指针类型的变量才可以实现想要的结果）
void DmsClient::startClient(){
	showdata->append("start client");//append()函数用于追加显示内容
/*	
	showdata->append("send data to server");
	sleep(1);
	showdata->append("send data to server");
	sleep(1);
	showdata->append("send data to server");
	sleep(1);
	showdata->append("send over");
*//*将注释的部分移动到线程处理函数中*/
//创建线程(用线程来处理耗时的操作，以免界面被冻结而无法进行其他操作,以及即使显示数据的问题也得到了解决)
  /* pthread_t thid;
	pthread_create(&thid,0,processData,showdata);
	*/
/*启动线程*/
	myth->start();
	/*访问界面*/
	/*发送数据*/
	/*访问界面*/
	/*对于多线程，专门会有一个线程来处理界面，而对于单线程，只有当该线程函数执行完后才重新刷新界面*/
	/*一启动按钮，按钮就处于无效状态*/
	bstart->setDisabled(true);
}
//在头文件中出现自定义的信号和槽函数时，要加宏Q_OBJECT
void DmsClient::getData(QString data){
	showdata->append(data);
}
