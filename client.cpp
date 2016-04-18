//
// Created by lishunan on 16-4-2.
//

#include "HostSvcCommon.h"
#include "example.pb.h"
#include "RPCController.h"

using asio::ip::tcp;
using namespace GkcHostSvc;
const static std::string server("127.0.0.1");



protobuf::RpcChannel* channel;
protobuf::RpcController* controller;
EchoService* service;
FooRequest request;
FooResponse response;

class MyRpcChannel :public protobuf::RpcChannel
{
private:
	p_socket _socket;
	std::vector<char> _request;
public:
	MyRpcChannel(){}
	~MyRpcChannel(){}
	// Call the given method of the remote service.  The signature of this
	// procedure looks the same as Service::CallMethod(), but the requirements
	// are less strict in one important way:  the request and response objects
	// need not be of any specific class as long as their descriptors are
	// method->input_type() and method->output_type().
	virtual void CallMethod(const protobuf::MethodDescriptor* method,
	                        protobuf::RpcController* controller,
		const protobuf::Message* request,
	                        protobuf::Message* response,
	                        protobuf::Closure* done){


		PackedMessage<FooRequest> pm;

		pm.set_msg(dynamic_cast<const FooRequest*> (request));
		pm.pack(_request);

		asio::error_code ec;
		asio::write(*_socket,asio::buffer(_request),ec);
		done->Run();
//		asio::async_write(*_socket,asio::buffer(_request),[&done](const asio::error_code& ec, std::size_t bytes_transferred)
//		{
//			done->Run();
//		});
	};
	void SetSocket(p_socket p)
	{
		_socket=p;
	}
};


void Done() {
	delete service;
	delete channel;
	delete controller;
}
void DoSearch() {
	// You provide classes MyRpcChannel and MyRpcController, which implement
	// the abstract interfaces protobuf::RpcChannel and protobuf::RpcController.
	channel = new MyRpcChannel();
	controller = new MyRpcController();

	// The protocol compiler generates the SearchService class based on the
	// definition given above.
	service = new EchoService::Stub(channel);

	// Set up the request.
	request.set_text("protocol buffers");

	// Execute the RPC.
	service->Foo(controller, &request, &response, protobuf::internal::NewCallback(&Done));
}



void get_socket_and_connection(p_socket& ps)
{
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
		echo_clt.Foo(&controller,&request,&response, google::protobuf::internal::NewCallback([](){ std::cout<<"done"<<std::endl;}));
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
