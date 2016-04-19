//
// Created by lishunan on 16-4-2.
//

#include "HostSvcCommon.h"
#include "RPCController.h"
#include "RPCChannel.h"

const static std::string server("127.0.0.1");

void get_socket_and_connection(p_socket &ps) {
    asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(server, PORT);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    ps = std::make_shared<tcp::socket>(io_service);

    asio::connect(*ps.get(), endpoint_iterator);
}

int main() {
    try {
        p_socket ps;
        get_socket_and_connection(ps);
        MyRpcChannel rpcChannel;
        rpcChannel.SetSocket(ps);
        EchoService_Stub echo_clt(&rpcChannel);
        FooRequest request;
        request.set_text("test1");
        request.set_times(1);

        FooResponse response;
        MyRpcController controller;
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
