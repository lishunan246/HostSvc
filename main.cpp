#include <iostream>
#include "HostSvcCommon.h"

namespace GkcHostSvc {
    using asio::ip::tcp;

	class Person_skeleton:public tutorial::Person
	{
		
	};

    class Client {
		tcp::socket socket;
		std::array<char, 10000> receive_buf{};
		std::array<char, 10000> msg{ "Welcome" };
		p_aint p;

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

        void start() {

            asio::async_write(socket, asio::buffer(msg),
                              [](const asio::error_code &errorCode, std::size_t bytes_transferred) {
				
                              });
			asio::async_read(socket,asio::buffer(receive_buf), [](const asio::error_code &errorCode, std::size_t bytes_transferred) {

			});
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
    ClientManager clientManager(io_service);
    clientManager.start_accept();
    io_service.run();
    return 0;
}
