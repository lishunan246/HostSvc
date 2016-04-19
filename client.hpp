#include "HostSvcCommon.h"
#include "example.pb.h"
#include "RPCController.h"

namespace GkcHostSvc {
    class Client : public std::enable_shared_from_this<Client> {
        tcp::socket socket;
        std::vector<char> read_buf,write_buf;
        p_aint p;
        std::shared_ptr<GkcHostSvc::EchoServiceImpl> _service;
        PackedMessage<FooRequest> m_packed_request;
    public:

        using pClient = std::shared_ptr<Client>;


        Client &operator=(const Client &) = delete;

        Client &operator=(Client &&) = default;

        explicit Client(asio::io_service &io_service, p_aint c, std::shared_ptr<GkcHostSvc::EchoServiceImpl> _s)
                : socket(io_service), p(c), _service(_s) {
            std::cout << "waiting" << std::endl;
            ++(*p);
        }

        Client(const Client &) = delete;

        Client(Client &&) = default;

        ~Client() {
            --*p;
            std::cout << "disconnecting" << std::endl;
        }

        static pClient create(asio::io_service &io_service, p_aint p, std::shared_ptr<GkcHostSvc::EchoServiceImpl> _s) {
            return std::make_shared<Client>(io_service, p, _s);
        }

        tcp::socket &getSocket() {
            return socket;
        }

        void handle_request() {

                auto msg=m_packed_request.unpack(read_buf);
                RPCController controller;
                FooResponse response;
                auto request=m_packed_request.get_msg();
                _service->Foo(&controller,request,&response, nullptr);
                PackedMessage<FooResponse> p;
                p.set_msg(&response);
                p.pack(write_buf);
                asio::write(socket, asio::buffer(write_buf));

        }

        void handle_read_body(const asio::error_code ec) {
            if (!ec) {

                handle_request();
                start_read_header();
            }
        };

        void start_read_body(unsigned msg_len) {
            read_buf.resize(HEADER_SIZE+msg_len);
            auto self(shared_from_this());
            asio::async_read(socket, asio::buffer(&read_buf[HEADER_SIZE],msg_len),
                             [self](const asio::error_code &ec, std::size_t bytes_transferred) {
                                 self->handle_read_body(ec);
                             });
        };

        void handle_read_header(const asio::error_code &ec) {

            if (!ec) {
                unsigned msg_len = m_packed_request.decode_header(read_buf);

                start_read_body(msg_len);
            }
        };

        void start_read_header() {
            read_buf.resize(HEADER_SIZE);
            auto self(shared_from_this());
            asio::async_read(socket, asio::buffer(read_buf),
                             [self](const asio::error_code &ec, std::size_t bytes_transferred) {
                                 self->handle_read_header(ec);
                             });

        };

        void start()
        {
            cout<<"connecting"<<endl;
            start_read_header();
       }
    };
}