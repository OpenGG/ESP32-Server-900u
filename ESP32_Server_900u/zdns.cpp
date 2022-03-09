#include <DNSServer.h>

static DNSServer dnsServer;

namespace zdns
{
    void setup(const IPAddress &Server_IP)
    {
        dnsServer.setTTL(30);
        dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
        dnsServer.start(53, "*", Server_IP);
    }

    void loop() {
        dnsServer.processNextRequest();
    }
}