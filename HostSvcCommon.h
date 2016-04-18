//
// Created by lishunan on 16-4-2.
//

#ifndef HOSTSVC_HOSTSVCCOMMON_H
#define HOSTSVC_HOSTSVCCOMMON_H

#define ASIO_STANDALONE
#define ASIO_HAS_STD_ADDRESSOF
#define ASIO_HAS_STD_ARRAY
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_STD_TYPE_TRAITS
#define ASIO_NO_TYPEID

#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif

#include <iostream>
#include <asio.hpp>
#include <string>
#include <atomic>
#include <utility>

//#include "RPCRequest.pb.h"
//#include "RPCResponse.pb.h"
#include "packedmessage.h"
#include "example.pb.h"

using namespace google;
namespace GkcHostSvc {
	using asio::ip::tcp;
	using namespace std;
//	using Request = GKC::RPCRequest;
//	using Response = GKC::RPCResponse;
    static const int PORT_INT(10000);
    static const std::string PORT("10000");
	using a_int = std::atomic<int>;
	using p_aint = std::shared_ptr<a_int>;
	using p_socket = std::shared_ptr<asio::ip::tcp::socket>;

	class EchoServiceImpl : public EchoService {

	public:

		EchoServiceImpl() {}

		virtual void Foo(::google::protobuf::RpcController* controller,

						 const ::FooRequest* request,

						 ::FooResponse* response,

						 ::google::protobuf::Closure* done) {
			std::cout<<" str:"<<std::endl;
			std::string str = request->text();
			std::cout<<" str:"<<str<<std::endl;

			std::string tmp = str;

			for (int i = 1; i < request->times(); i++)

				str += (" " + tmp);

			response->set_text(str);

			response->set_result(true);

		}

	};
}

#endif //HOSTSVC_HOSTSVCCOMMON_H
