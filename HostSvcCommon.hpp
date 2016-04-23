//
// Created by lishunan on 16-4-2.
//

#ifndef HOSTSVC_HOSTSVCCOMMON_H
#define HOSTSVC_HOSTSVCCOMMON_H

#ifdef _MSC_VER
#define ASIO_STANDALONE
#define ASIO_HAS_STD_ADDRESSOF
#define ASIO_HAS_STD_ARRAY
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_STD_TYPE_TRAITS
#define ASIO_NO_TYPEID

#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif

#include <iostream>
#include <asio.hpp>
#include <string>
#include <atomic>
#include <utility>

#include "packedmessage.hpp"
#include "example.pb.h"

namespace GkcHostSvc {
	using asio::ip::tcp;
	using namespace std;
	using namespace google;
    static const int PORT_INT(10000);
    static const std::string PORT("10000");

	using atomicInt = std::atomic<int>;
	using pAtomicInt = std::shared_ptr<atomicInt>;
	using pSocket = std::shared_ptr<asio::ip::tcp::socket>;

}

#endif //HOSTSVC_HOSTSVCCOMMON_H
