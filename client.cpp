//
// Created by lishunan on 16-4-2.
//

#include "HostSvcCommon.h"
#include "RPCChannel.h"
#include "TestObject.h"
const static std::string server("127.0.0.1");


int main() {
    try {
        Connection rpcChannel(server,PORT);

        ProxyObject counter(&rpcChannel,1);
        counter.add(100);
        counter.sub(10);
        ProxyObject counter2(&rpcChannel,3);
        counter2.sub(133);
        std::cout<<"result 2: ";
        std::cout<<counter2.getCount()<<std::endl;
        std::cout<<"result: ";
        std::cout<<counter.getCount()<<std::endl;

    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
#ifdef WIN32
    system("pause");
#endif

    return 0;
}
