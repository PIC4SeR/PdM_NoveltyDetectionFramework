#include "../include/INotifyDriverAgentInterface.h"
#include <boost/asio.hpp>
#include <cinttypes>


using namespace Spea::Archimede::MultiAgentSystem::Tools;

INotifyDriverAgentInterface::INotifyDriverAgentInterface() {}
INotifyDriverAgentInterface::INotifyDriverAgentInterface(string pIpAddress, uint16_t pPort) {
	this->rmIpAddress = pIpAddress;
	this->rmPort = pPort;
}
INotifyDriverAgentInterface::~INotifyDriverAgentInterface() {}
int INotifyDriverAgentInterface::fSendNotification(string pNotificationMessage) {
    bool rConnected = false;
    boost::asio::io_service cIOservice;
    boost::system::error_code cError;
    boost::asio::ip::tcp::socket *cSocket;
    boost::asio::ip::tcp::resolver::query *cDNSResolverQuery;
    boost::asio::ip::tcp::resolver* cDNSResolver;
    boost::asio::ip::tcp::resolver::iterator cDNSResolverIterator;
    boost::asio::ip::tcp::resolver::iterator cDNSResolverEndIterator;

    try {
        cSocket = new boost::asio::ip::tcp::socket(cIOservice);
        cDNSResolverQuery = new boost::asio::ip::tcp::resolver::query(this->rmIpAddress, to_string(this->rmPort), boost::asio::ip::tcp::resolver::query::numeric_service);
        cDNSResolver = new  boost::asio::ip::tcp::resolver(cIOservice);
    } catch (boost::system::system_error e) {
        fprintf(stdout, "[fSendNotification] Socket and Resolver configuration fails - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
        fflush(stdout);
        return(TcError::kSendNotification_ConfigurationFail);
    }

    try {
        cDNSResolverIterator = cDNSResolver->resolve(*cDNSResolverQuery, cError);
        if (cError) {
            fprintf(stdout, "[fSendNotification] Resolve quey fails - Message %s Value %d Category %s\n", cError.message().c_str(), cError.value(), cError.category().name());
            fflush(stdout);
            return(TcError::kSendNotification_DNSResolveFail);
        }
    } catch (boost::system::system_error e) {
        fprintf(stdout, "[fSendNotification] Resolve query fails - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
        fflush(stdout);
        return(TcError::kSendNotification_DNSResolveFail);
    }

    
    for (; cDNSResolverIterator != cDNSResolverEndIterator; cDNSResolverIterator++) {
        boost::asio::ip::tcp::endpoint cEndPoint = cDNSResolverIterator->endpoint();
        try {
            cSocket->connect(cEndPoint, cError);
            if (cError) {
                rConnected = false;
                continue;
            } else {
                rConnected = true;
                break;
            }
        } catch (boost::system::system_error e) {
            fprintf(stdout, "[fSendNotification] Connect fails - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
            fflush(stdout);
            rConnected = false;
        }
    }

    if (!rConnected) {
        fprintf(stdout, "[fSendNotification] Connection to IP Notifier fails\n");
        fflush(stdout);
        return(TcError::kSendNotification_ConnectionFail);
    }

    cSocket->shutdown(boost::asio::socket_base::shutdown_type::shutdown_receive);
    cSocket->wait(boost::asio::ip::tcp::socket::wait_write);

    boost::asio::write(*cSocket, boost::asio::buffer(pNotificationMessage), cError);
    
    cSocket->shutdown(boost::asio::socket_base::shutdown_type::shutdown_both);
    cSocket->release();
    cSocket->close();

    if (cError) {
        fprintf(stdout, "[fSendNotification] Send notification message fails - Message %s Value %d Category %s\n", cError.message().c_str(), cError.value(), cError.category().name());
        fflush(stdout);
        return(TcError::kSendNotification_SendFail);
    }
    return(TcError::kSendNotification_Success);
}