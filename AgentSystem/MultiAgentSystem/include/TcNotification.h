#ifndef TCNOTIFICATION_H
#define TCNOTIFICATION_H

#include <string>
#include <chrono>
#include <boost/asio.hpp>



using namespace std;


class TcNotification {

private:
	//Notification Message composition
	string rmType;
	string rmTitle;
	string rmBody;

public:

	class Type {
		const string kAlarm = "Alarm";
		const string kWarning = "Warning";
		const string kReport = "Report";
		const string kInfo = "Info";
	};


	TcNotification();
	TcNotification(string pType, string pTitle, string pBody);
	~TcNotification();

	string fGetNotificationString();


};


#endif