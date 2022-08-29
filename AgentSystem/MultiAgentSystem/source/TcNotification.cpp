#include "../include/TcNotification.h"

TcNotification::TcNotification() {}
TcNotification::~TcNotification() {}
TcNotification::TcNotification(string pType, string pTitle, string pBody) {
    this->rmType = pType;
    this->rmTitle = pTitle;
    this->rmBody = pBody;
}
string TcNotification::fGetNotificationString() {
    return string("");
}









