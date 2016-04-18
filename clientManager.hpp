#include "HostSvcCommon.h"
#include "client.hpp"
namespace GkcHostSvc
{
	class ClientManager {
		tcp::acceptor acceptor;
		p_aint pCount = std::make_shared<a_int>(0);
		std::shared_ptr<GkcHostSvc::EchoServiceImpl> _s;

	public:
		explicit ClientManager(asio::io_service &io_service) : acceptor(io_service, tcp::endpoint(tcp::v4(), PORT_INT)) {}
		ClientManager(const ClientManager&) = delete;
		ClientManager(ClientManager&&) = default;
		ClientManager& operator=(const ClientManager&) = delete;
		ClientManager& operator=(ClientManager&&) = default;

		void start_accept();

		void handle_accept(Client::pClient pClient, const asio::error_code &errorCode);
		bool registerService(std::shared_ptr<GkcHostSvc::EchoServiceImpl> s)
		{
			_s=s;
		}

	};

	void ClientManager::start_accept() {
		auto newClient = Client::create(acceptor.get_io_service(), pCount,_s);

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