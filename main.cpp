#include <iostream>
#include "HostSvcCommon.h"

namespace GkcHostSvc {
    using asio::ip::tcp;
    std::string msg("Welcome");

    class Client {
    public:
        using pClient=std::shared_ptr<Client>;

        Client(asio::io_service &io_service) : socket(io_service) {

        }

        static pClient create(asio::io_service &io_service) {
            return std::make_shared<Client>(io_service);
        }

        tcp::socket &getSocket() {
            return socket;
        }

        void start() {

            asio::async_write(socket, asio::buffer(msg),
                              [](const asio::error_code &errorCode, std::size_t bytes_transferred) {

                              });
        }

    private:
        tcp::socket socket;
    };

    class ClientManager {
    private:
        tcp::acceptor acceptor;
    public:
        ClientManager(asio::io_service &io_service) : acceptor(io_service, tcp::endpoint(tcp::v4(), PORT_INT)) {
        }

        void start_accept();

        void handle_accept(Client::pClient pClient, const asio::error_code &errorCode);
    };

    void ClientManager::start_accept() {
        auto newClient = Client::create(acceptor.get_io_service());
        acceptor.async_accept(newClient->getSocket(), [this, newClient](const asio::error_code &ec) {
            this->handle_accept(newClient, ec);
        });
    }

    void ClientManager::handle_accept(Client::pClient pClient, const asio::error_code &errorCode) {
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