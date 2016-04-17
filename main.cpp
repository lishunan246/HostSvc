#include <iostream>
#include "HostSvcCommon.h"
#include "clientManager.hpp"

using namespace GkcHostSvc;
using namespace google::protobuf;
class EchoServiceImpl : public EchoService {
public:
	EchoServiceImpl() {}
	virtual void Foo(::google::protobuf::RpcController* controller,
		const ::FooRequest* request,
		::FooResponse* response,
		::google::protobuf::Closure* done) {
		std::string str = request->text();
		std::string tmp = str;
		for (int i = 1; i < request->times(); i++)
			str += (" " + tmp);
		response->set_text(str);
		response->set_result(true);
	}
};
int main() {
    asio::io_service io_service;
	std::cout << sizeof(char) << std::endl;
	std::cout << sizeof(int64_t) << std::endl;
    ClientManager clientManager(io_service);
    clientManager.start_accept();
    io_service.run();
    return 0;
}
