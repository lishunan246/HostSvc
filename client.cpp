//
// Created by lishunan on 16-4-2.
//

#include "HostSvcCommon.h"
#include "example.pb.h"

using asio::ip::tcp;
using namespace GkcHostSvc;
const static std::string server("127.0.0.1");

using namespace google;

protobuf::RpcChannel* channel;
protobuf::RpcController* controller;
EchoService* service;
FooRequest request;
FooResponse response;

class MyRpcChannel :public protobuf::RpcChannel
{
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
	                        protobuf::Closure* done){};
};

class MyRpcController:public protobuf::RpcController
{
public:
	MyRpcController()
	{
	};
	~MyRpcController()
	{
		
	}
	// Client-side methods ---------------------------------------------
	// These calls may be made from the client side only.  Their results
	// are undefined on the server side (may crash).

	// Resets the RpcController to its initial state so that it may be reused in
	// a new call.  Must not be called while an RPC is in progress.
	 void Reset() {};

	// After a call has finished, returns true if the call failed.  The possible
	// reasons for failure depend on the RPC implementation.  Failed() must not
	// be called before a call has finished.  If Failed() returns true, the
	// contents of the response message are undefined.
	bool Failed() const
	{
		return false;
	};

	// If Failed() is true, returns a human-readable description of the error.
	std::string ErrorText() const
	{
		return "123";
	};

	// Advises the RPC system that the caller desires that the RPC call be
	// canceled.  The RPC system may cancel it immediately, may wait awhile and
	// then cancel it, or may not even cancel the call at all.  If the call is
	// canceled, the "done" callback will still be called and the RpcController
	// will indicate that the call failed at that time.
	void StartCancel(){};

	// Server-side methods ---------------------------------------------
	// These calls may be made from the server side only.  Their results
	// are undefined on the client side (may crash).

	// Causes Failed() to return true on the client side.  "reason" will be
	// incorporated into the message returned by ErrorText().  If you find
	// you need to return machine-readable information about failures, you
	// should incorporate it into your response protocol buffer and should
	// NOT call SetFailed().
	void SetFailed(const std::string& reason){};

	// If true, indicates that the client canceled the RPC, so the server may
	// as well give up on replying to it.  The server should still call the
	// final "done" callback.
	bool IsCanceled() const
	{
		return false;
	};

	// Asks that the given callback be called when the RPC is canceled.  The
	// callback will always be called exactly once.  If the RPC completes without
	// being canceled, the callback will be called after completion.  If the RPC
	// has already been canceled when NotifyOnCancel() is called, the callback
	// will be called immediately.
	//
	// NotifyOnCancel() must be called no more than once per request.
	void NotifyOnCancel(protobuf::Closure* callback) {};
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

	    
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
#ifdef WIN32
	system("pause");
#endif

    return 0;
}
