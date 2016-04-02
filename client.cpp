//
// Created by lishunan on 16-4-2.
//

#include "HostSvcCommon.h"

using asio::ip::tcp;
using namespace GkcHostSvc;
const static std::string server("127.0.0.1");

int main() {
    try {
        asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(server, PORT);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        tcp::socket socket(io_service);
        asio::connect(socket, endpoint_iterator);

        std::array<char, 128> buf;
        asio::error_code error;

        size_t len = socket.read_some(asio::buffer(buf), error);

           

       std::cout.write(buf.data(), len);

	   
        
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
#ifdef WIN32
	system("pause");
#endif

    return 0;
}