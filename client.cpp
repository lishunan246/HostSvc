//
// Created by lishunan on 16-4-2.
//

#include "HostSvcCommon.hpp"
#include "RPCChannel.hpp"
#include "TestObject.h"

const static std::string server("127.0.0.1");


int main()
{
    try {
        Connection rpcChannel(server,PORT);

        ProxyObject counter(&rpcChannel,1);
        ProxyStringObject string(&rpcChannel,2);
        counter.add(100);
        counter.sub(10);
        string.append("1232");
        ProxyObject counter2(&rpcChannel,3);
        counter2.sub(133);
        std::cout<<"id : "<<rpcChannel.getClientID()<<endl<<"result 2: ";
        std::cout<<counter2.getCount()<<std::endl;
        std::cout<<"id : "<<rpcChannel.getClientID()<<endl<<"result: ";
        std::cout<<counter.getCount()<<std::endl;
        std::cout<<"string: "<<string.get()<<endl;

    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
#ifdef WIN32
    system("pause");
#endif

    return 0;
}
