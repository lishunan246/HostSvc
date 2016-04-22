#include "HostSvcCommon.h"
#include "example.pb.h"
#include "RPCController.h"
#include "ServiceImpl.h"

namespace GkcHostSvc {
    class Client : public std::enable_shared_from_this<Client> {
        tcp::socket _socket;
        std::vector<char> _read_buf,_write_buf;
        p_aint _pCounter;
        std::shared_ptr<GkcHostSvc::EchoServiceImpl> _service;
        PackedMessage<RPCRequest> _packedRequest;
        PackedMessage<RPCResponse> _packedResponse;
        int _id;

        pObjectMap _ObjectMap=std::make_shared<ObjectMap>();
    public:
        using pClient = std::shared_ptr<Client>;

        Client &operator=(const Client &) = delete;
        Client &operator=(Client &&) = default;

        explicit Client(asio::io_service &io_service, p_aint c, std::shared_ptr<GkcHostSvc::EchoServiceImpl> _s)
                : _socket(io_service), _pCounter(c), _service(_s) {
            std::cout << " waiting" << std::endl;
            ++(*_pCounter);
            _s->setMap(_ObjectMap);
        }

        Client(const Client &) = delete;

        Client(Client &&) = default;

        ~Client() {
            --*_pCounter;
            std::cout << _id<<"disconnecting" << std::endl;
        }

        static pClient create(asio::io_service &io_service, p_aint p, pService _s) {
            return std::make_shared<Client>(io_service, p, _s);
        }

        tcp::socket &getSocket() {
            return _socket;
        }

        void setID(int x)
        {
            _id=x;
        }

        void handle_request() {

                auto msg=_packedRequest.unpack(_read_buf);
                RPCController controller;
                RPCResponse response;
                response.set_clientid(_id);
                auto request=_packedRequest.get_msg();
                _service->RPC(&controller,request,&response, nullptr);

                _packedResponse.set_msg(&response);
                _packedResponse.pack(_write_buf);
                asio::write(_socket, asio::buffer(_write_buf));

        }

        void handle_read_body(const asio::error_code ec) {
            if (!ec) {

                handle_request();
                start_read_header();
            }
        };

        void start_read_body(unsigned msg_len) {
            _read_buf.resize(HEADER_SIZE+msg_len);
            auto self(shared_from_this());
            asio::async_read(_socket, asio::buffer(&_read_buf[HEADER_SIZE],msg_len),
                             [self](const asio::error_code &ec, std::size_t bytes_transferred) {
                                 self->handle_read_body(ec);
                             });
        };

        void handle_read_header(const asio::error_code &ec) {

            if (!ec) {
                unsigned msg_len = _packedRequest.decode_header(_read_buf);

                start_read_body(msg_len);
            }
        };

        void start_read_header() {
            _read_buf.resize(HEADER_SIZE);
            auto self(shared_from_this());
            asio::async_read(_socket, asio::buffer(_read_buf),
                             [self](const asio::error_code &ec, std::size_t bytes_transferred) {
                                 self->handle_read_header(ec);
                             });

        };

        void start()
        {
            cout<<_id<<" connecting"<<endl;
            start_read_header();
       }
    };
}