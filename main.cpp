#include <iostream>
#include "HostSvcCommon.h"
#include "clientManager.hpp"
#include "example.pb.h"

using namespace GkcHostSvc;
using namespace google::protobuf;

int main() {
	auto impl=std::make_shared<EchoServiceImpl>();
    asio::io_service io_service;
	std::cout << sizeof(char) << std::endl;
	std::cout << sizeof(int64_t) << std::endl;
    ClientManager clientManager(io_service);
    clientManager.registerService(impl);
    clientManager.start_accept();

    io_service.run();
    return 0;
}
