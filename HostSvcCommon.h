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

namespace GkcHostSvc {
    static const int PORT_INT(10000);
    static const std::string PORT("10000");
}

#endif //HOSTSVC_HOSTSVCCOMMON_H
