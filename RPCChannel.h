//
// Created by lishunan on 4/19/16.
//

#ifndef HOSTSVC_RPCCHANNEL_H
#define HOSTSVC_RPCCHANNEL_H

#include "HostSvcCommon.h"

using namespace GkcHostSvc;
using namespace protobuf;

class Connection : public protobuf::RpcChannel {
private:
    asio::io_service _io_service;
    tcp::resolver _resolver;
    tcp::resolver::query _query;
    tcp::resolver::iterator _endpoint_iterator;
    tcp::socket _socket;
    std::vector<char> _request, _response;
    PackedMessage<RPCRequest> _packedRequest;
    PackedMessage<RPCResponse> _packedResponse;
    int _clientID=-1;
public:
    Connection(const string& server,const string& PORT) :_resolver(_io_service),_query(server, PORT),_endpoint_iterator(_resolver.resolve(_query)),_socket(_io_service)
    {
        asio::connect(_socket, _endpoint_iterator);
    }

    int getClientID()
    {
        return _clientID;
    }

    ~Connection() { }

    virtual void CallMethod(const MethodDescriptor *method,RpcController *controller, const Message *request, Message *response, Closure *done)
    {
        auto p = dynamic_cast<const RPCRequest *> (request);

        _packedRequest.set_msg(p);
        _packedRequest.pack(_request);

        asio::error_code ec;
        asio::write(_socket,asio::buffer(_request), ec);

        _response.resize(HEADER_SIZE);
        asio::read(_socket, asio::buffer(_response), ec);

        auto size = _packedResponse.decode_header(_response);
        _response.resize(size + HEADER_SIZE);
        asio::read(_socket, asio::buffer(&_response[HEADER_SIZE], size), ec);

        _packedResponse.unpack(_response);
        auto pResponse = const_cast<RPCResponse *>(_packedResponse.get_msg());
        //auto text = *(pResponse->());
        _clientID=pResponse->clientid();

        auto pLocalResponse = static_cast<RPCResponse *>(response);

        *pLocalResponse=*pResponse;
//        pLocalResponse->set_success(true);
//        pLocalResponse->set_resultint(pResponse->resultint());
//        pLocalResponse->set_clientid(pResponse->clientid());
        if(done!= nullptr)
            done->Run();
    };
};

#endif //HOSTSVC_RPCCHANNEL_H
