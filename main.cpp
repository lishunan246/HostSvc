#include <iostream>
#include "HostSvcCommon.h"

namespace GkcHostSvc {
    using asio::ip::tcp;

	class Person_skeleton:public tutorial::Person
	{
		
	};

    class Client :public std::enable_shared_from_this<Client>{
		tcp::socket socket;
		std::array<char, 10000> receive_buf{};
		std::array<char, 10000> msg{ "Welcome" };
		data_buffer read_buf;
		p_aint p;
		PackedMessage<Request> packed_request;

    public:
        using pClient=std::shared_ptr<Client>;


		Client& operator=(const Client&) = delete;
		Client& operator=(Client&&) = default;
		explicit Client(asio::io_service &io_service, p_aint c) : socket(io_service), p(c)
		{
			std::cout << "connecting" << std::endl;
			++(*p);
		}
		Client(const Client&) = delete;
		Client(Client&&) = default;
		~Client(){
			--*p;
			std::cout << "disconnecting" << std::endl;
		}

        static pClient create(asio::io_service &io_service,p_aint p) {
            return std::make_shared<Client>(io_service,p);
        }

        tcp::socket &getSocket() {
            return socket;
        }

	    void handle_request()
		{
			if(packed_request.unpack(read_buf))
			{
				auto req = packed_request.get_msg();

			}
		}

	    void handle_read_body(const asio::error_code ec)
		{
			if(!ec)
			{
				start_read_header();
				handle_request();
			}
		};

	    void start_read_body(unsigned msg_len)
		{
			read_buf.resize(HEADER_SIZE + msg_len);
			auto self(shared_from_this());
			asio::async_read(socket, asio::buffer(read_buf), [self](const asio::error_code& ec, std::size_t bytes_transferred)
			{
				self->handle_read_body(ec);
			});
		};

	    void handle_read_header(const asio::error_code& ec)
		{
			if(!ec)
			{
				auto msg_len = PackedMessage<Request>::decode_header(read_buf);
				start_read_body(msg_len);
			}
		};

	    void start_read_header()
		{
			read_buf.resize(HEADER_SIZE);
			auto self(shared_from_this());
			asio::async_read(socket, asio::buffer(read_buf), [self](const asio::error_code& ec, std::size_t bytes_transferred)
			{
				self->handle_read_header(ec);
			});
		};

	    void start() {
			start_read_header();
        }
    };

    class ClientManager {
	    tcp::acceptor acceptor;
		p_aint pCount = std::make_shared<a_int>(0);

    public:
	    explicit ClientManager(asio::io_service &io_service) : acceptor(io_service, tcp::endpoint(tcp::v4(), PORT_INT)) {}
		ClientManager(const ClientManager&) = delete;
		ClientManager(ClientManager&&) = default;
		ClientManager& operator=(const ClientManager&) = delete;
		ClientManager& operator=(ClientManager&&) = default;

        void start_accept();

        void handle_accept(Client::pClient pClient, const asio::error_code &errorCode);
    };

    void ClientManager::start_accept() {
        auto newClient = Client::create(acceptor.get_io_service(),pCount);

        acceptor.async_accept(newClient->getSocket(), [this, newClient](const asio::error_code &ec) {

            this->handle_accept(newClient, ec);
        });
    }

    void ClientManager::handle_accept(Client::pClient pClient, const asio::error_code &errorCode) {
		std::cout << *pCount << std::endl;

        if (!errorCode)
            pClient->start();

        start_accept();
    }


}

using namespace GkcHostSvc;

int main() {
    asio::io_service io_service;
	std::cout << sizeof(char) << std::endl;
	std::cout << sizeof(int64_t) << std::endl;
    ClientManager clientManager(io_service);
    clientManager.start_accept();
    io_service.run();
    return 0;
}
