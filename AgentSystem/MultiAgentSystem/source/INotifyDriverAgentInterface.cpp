#include "../include/INotifyDriverAgentInterface.h"

INotifyDriverAgentInterface::INotifyDriverAgentInterface() {}
INotifyDriverAgentInterface::~INotifyDriverAgentInterface() {}
int INotifyDriverAgentInterface::fSendNotification(string pNotificationMessage) {
    return(TcError::kSendNotification_Success);
}