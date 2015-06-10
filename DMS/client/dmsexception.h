#ifndef DMSEXCEPTION_H
#define DMSEXCEPTION_H
//c++中规定任何一个类都可以充当异常，所以不必让异常类继承于exception类
#include<string.h>
class DmsClientException{
private:
	char msg[256];//用于保存异常的消息
public:
	DmsClientException(){
		strcpy(msg,"dms client exception");
	}//在默认构造函数中传入了一个默认消息
	DmsClientException(const char* msg){
		strcpy(this->msg,msg);
	}//在有参构造中，可将自己想要的消息传入
	//为外部提供一个消息函数,抛出任何异常
	virtual const char* what()const throw(){
		return msg;
	}
};//客户端异常类

class DmsSenderException:public DmsClientException{
public:
	DmsSenderException():DmsClientException("dms sender exception"){
		
	}
	DmsSenderException(const char *msg):DmsClientException(msg){
	}
	/*const char* what()const throw(){
		return "this is child exception";
	}*/
};

class DmsInitNetWorkException:public DmsSenderException{
	public:
	DmsInitNetWorkException(const char* msg):DmsSenderException(msg){}
	DmsInitNetWorkException():DmsSenderException("dms initnetwork exception"){
	}
};

class DmsSendDataException:public DmsSenderException{
	public:
	DmsSendDataException(const char* msg):DmsSenderException(msg){}
	DmsSendDataException():DmsSenderException("dms senderdata exception"){
	}
};
#endif


