#ifndef INOTIFYDRIVERAGENTINTERFACE_H
#define INOTIFYDRIVERAGENTINTERFACE_H

#include <string>
#include <chrono>
#include <cstdint>


using namespace std;


class INotifyDriverAgentInterface {
private:

	class TcError {
	public:
		static const int8_t kSendNotification_ConfigurationFail = -1;
		static const int8_t kSendNotification_DNSResolveFail = -2;
		static const int8_t kSendNotification_ConnectionFail = -3;
		static const int8_t kSendNotification_SendFail = -4;
		static const int8_t kSendNotification_Success = 0;
	};

public:



	INotifyDriverAgentInterface();
	~INotifyDriverAgentInterface();

	int fSendNotification(string pNotificationMessage);
};
			


#endif