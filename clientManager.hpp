#include "HostSvcCommon.hpp"
#include "client.hpp"

namespace GkcHostSvc {
        class ClientManager {
        tcp::acceptor _acceptor;
        pAtomicInt pCount = std::make_shared<atomicInt>(0);
        pService _s;
        atomic<int> clientCount;
    public:
        explicit ClientManager(asio::io_service &io_service) : _acceptor(io_service,
                                                                        tcp::endpoint(tcp::v4(), PORT_INT)) {
            clientCount = 0;
        }

        ClientManager(const ClientManager &) = delete;

        ClientManager(ClientManager &&) = default;

        ClientManager &operator=(const ClientManager &) = delete;

        ClientManager &operator=(ClientManager &&) = default;

        void handle_accept(Client::pClient pClient, const asio::error_code &errorCode) {
            std::cout << *pCount << std::endl;

            if (!errorCode)
                pClient->start();

            start_accept();
        }

        bool registerService(pService s) {
            _s = s;
            return true;
        }

        void start_accept() {
            auto newClient = Client::create(_acceptor.get_io_service(), pCount, _s);
            newClient->setID(clientCount);
            ++clientCount;
            _acceptor.async_accept(newClient->getSocket(), [this, newClient](const asio::error_code &ec) {
                this->handle_accept(newClient, ec);
            });
        }
    };
}