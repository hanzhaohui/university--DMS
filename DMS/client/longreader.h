#ifndef LOGREADER_H
#define LOGREADER_H
#include<list>
#include "data.h"
using namespace std;//当编译通过后，就可将此句删掉，因为list是在namespace中定义的，刚开始编译时，因为没有写此句，编译无法通过，编译器不认识list
class LogReader{
private:
	char logFileName[50];
  	char backFileName[50];	
	char failLoginsFileName[50];
	list<LogRec> logins;
	list<LogRec> logouts;
   list<MatchedLogRec> matches;
	void backup();//将变化的文件变成不变的
	void readFailLogins();
	void readBackupFile();//读取备份文件
	void matchLogRec();
	void saveFailLogins();
public:	 
	 LogReader();
    ~LogReader();
   list<MatchedLogRec> readLogs(); //私有函数是私有函数的实现者（即用私有函数来实现公有函数）	
};
#endif
