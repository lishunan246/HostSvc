#include <iostream>
#include "HostSvcCommon.h"
#include "client.hpp"
#include "clientManager.hpp"

using namespace GkcHostSvc;
using namespace google::protobuf;
class ExampleSearchService : public tutorial::SearchService {
public:
	void Search(RpcController* controller,
		const tutorial::SearchRequest* request,
	            tutorial::SearchResponse* response,
		Closure* done) {
		if (request->request().size())
			response->set_response("123");
		done->Run();
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
