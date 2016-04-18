#include "HostSvcCommon.h"
#include "example.pb.h"
namespace GkcHostSvc
{
	class Client :public std::enable_shared_from_this<Client> {
		tcp::socket socket;
		std::array<char, 10000> receive_buf{};
		std::array<char, 10000> msg{ "Welcome" };
		data_buffer read_buf;
		p_aint p;
		

	public:
		using pClient = std::shared_ptr<Client>;


		Client& operator=(const Client&) = delete;
		Client& operator=(Client&&) = default;
		explicit Client(asio::io_service &io_service, p_aint c) : socket(io_service), p(c)
		{
			std::cout << "connecting" << std::endl;
			++(*p);
		}
		Client(const Client&) = delete;
		Client(Client&&) = default;
		~Client() {
			--*p;
			std::cout << "disconnecting" << std::endl;
		}

		static pClient create(asio::io_service &io_service, p_aint p) {
			return std::make_shared<Client>(io_service, p);
		}

		tcp::socket &getSocket() {
			return socket;
		}

		void handle_request()
		{
			
		}

		void handle_read_body(const asio::error_code ec)
		{
			if (!ec)
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
			if (!ec)
			{
				auto msg_len = PackedMessage<FooRequest>::decode_header(read_buf);
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
}