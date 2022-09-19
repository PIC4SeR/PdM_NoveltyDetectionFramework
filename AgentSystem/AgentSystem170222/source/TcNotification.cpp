#include "../include/TcNotification.h"

TcNotification::TcNotification() {}
TcNotification::~TcNotification() {}
TcNotification::TcNotification(string pType, string pTitle, string pBody, string pReferenceID, string pReferenceFamily, string pTestName) {
    this->rmType = pType;
    this->rmTitle = pTitle;
    this->rmBody = pBody;
    this->rmReferenceID = pReferenceFamily;
    this->rmReferenceFamily = pReferenceFamily;
    this->rmTestName = pTestName;

}
string TcNotification::fGetNotificationString(uint64_t pNotificationCounter, uint64_t pNotificationNumber) {
    bool rMultipleNotification = false;
    if (!(pNotificationCounter == 0 && pNotificationNumber == 0) && !(pNotificationCounter == 1 && pNotificationNumber == 1)) {
        rMultipleNotification = true;
    }
    return(this->rmType + "|0|" + this->rmTitle + "|" + this->rmBody + "|" 
            + (rMultipleNotification ? to_string(pNotificationCounter) + "/" + to_string(pNotificationNumber) : string()) + "|" 
            + (this->rmReferenceID.empty() ? "0" : this->rmReferenceID) + "|" + (this->rmReferenceFamily.empty() ? "0" : this->rmReferenceFamily) + "|" + (this->rmTestName.empty() ? "": this->rmTestName));
}









