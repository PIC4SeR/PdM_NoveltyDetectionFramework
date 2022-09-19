#ifndef TCNOTIFICATION_H
#define TCNOTIFICATION_H

#include <string>
#include <chrono>
#include <boost/asio.hpp>



using namespace std;


class TcNotification {

private:

	string rmType;
	string rmTitle;
	string rmBody;

	//Equipment Serial Number
	string rmReferenceID;
	//Equipment machine family
	string rmReferenceFamily;
	//TestName in case of notifications related to executed test. 
	string rmTestName;

public:

	class Type {
		const string kAlarm = "Alarm";
		const string kWarning = "Warning";
		const string kReport = "Report";
		const string kInfo = "Info";
	};



	TcNotification();
	TcNotification(string pType, string pTitle, string pBody, string pReferenceID, string pReferenceFamily, string pTestName);
	~TcNotification();

	string fGetNotificationString(uint64_t pNotificationCounter = 0, uint64_t pNotificationNumber = 0);

};



#endif