//
// Created by lishunan on 16-4-2.
//

#include "HostSvcCommon.h"

using asio::ip::tcp;
using namespace GkcHostSvc;
const static std::string server("127.0.0.1");

class Person_stub :public tutorial::Person
{
private:
	p_socket socket;
public:
	explicit Person_stub(p_socket psocket) :socket(psocket) {};
	void set_name(const ::std::string& value) {};
	void set_name(const char* value){};
	void set_id(int32_t value){};
	
};

int main() {
    try {
        asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(server, PORT);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		p_socket ps= std::make_shared<tcp::socket>(io_service);

        asio::connect(*ps.get(), endpoint_iterator);

        std::array<char, 128> buf;
        asio::error_code error;

        size_t len = (*ps.get()).read_some(asio::buffer(buf), error);

           

       std::cout.write(buf.data(), len);
	   Person_stub p(ps);
	   p.set_id(123);
	   p.set_name("sdf");

	   
        
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
#ifdef WIN32
	system("pause");
#endif

    return 0;
}