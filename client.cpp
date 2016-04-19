//
// Created by lishunan on 16-4-2.
//

#include "HostSvcCommon.h"
#include "RPCController.h"
#include "RPCChannel.h"

const static std::string server("127.0.0.1");


int main() {
    try {
        Connection rpcChannel(server,PORT);

        EchoService_Stub echo_clt(&rpcChannel);
        FooRequest request;
        request.set_text("test1");
        request.set_times(1);

        FooResponse response;
        RPCController controller;
        echo_clt.Foo(&controller, &request, &response,
                     google::protobuf::internal::NewCallback([]() { std::cout << "done" << std::endl; }));
        if (controller.Failed()) {
            printf("test 1 Rpc Call Failed : %s\n", controller.ErrorText().c_str());
        } else {
            printf("++++++ test 1 Rpc Response is %s\n", response.text().c_str());
        }
        echo_clt.Foo(&controller, &request, &response,
                     google::protobuf::internal::NewCallback([]() { std::cout << "done" << std::endl; }));
        if (controller.Failed()) {
            printf("test 1 Rpc Call Failed : %s\n", controller.ErrorText().c_str());
        } else {
            printf("++++++ test 1 Rpc Response is %s\n", response.text().c_str());
        }

    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
#ifdef WIN32
    system("pause");
#endif

    return 0;
}
