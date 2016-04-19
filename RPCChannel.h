//
// Created by lishunan on 4/19/16.
//

#ifndef HOSTSVC_RPCCHANNEL_H
#define HOSTSVC_RPCCHANNEL_H

#include "HostSvcCommon.h"

using namespace GkcHostSvc;

class Connection : public protobuf::RpcChannel {
private:
    asio::io_service _io_service;
    tcp::resolver _resolver;
    tcp::resolver::query _query;
    tcp::resolver::iterator _endpoint_iterator;
    tcp::socket _socket;
    std::vector<char> _request, _response;
    PackedMessage<FooRequest> _packedRequest;
    PackedMessage<FooResponse> _packedResponse;
public:
    Connection(const string& server,const string& PORT) :_resolver(_io_service),_query(server, PORT),_endpoint_iterator(_resolver.resolve(_query)),_socket(_io_service)
    {
        asio::connect(_socket, _endpoint_iterator);
    }

    ~Connection() { }

    // Call the given method of the remote service.  The signature of this
    // procedure looks the same as Service::CallMethod(), but the requirements
    // are less strict in one important way:  the request and response objects
    // need not be of any specific class as long as their descriptors are
    // method->input_type() and method->output_type().
    virtual void CallMethod(const protobuf::MethodDescriptor *method,
                            protobuf::RpcController *controller,
                            const protobuf::Message *request,
                            protobuf::Message *response,
                            protobuf::Closure *done) {


        auto p = dynamic_cast<const FooRequest *> (request);
        _packedRequest.set_msg(p);
        _packedRequest.pack(_request);

        asio::error_code ec;
        asio::write(_socket,asio::buffer(_request), ec);
        done->Run();
        _response.resize(HEADER_SIZE);
        asio::read(_socket, asio::buffer(_response), ec);

        auto size = _packedResponse.decode_header(_response);
        _response.resize(size + HEADER_SIZE);
        asio::read(_socket, asio::buffer(&_response[HEADER_SIZE], size), ec);
        _packedResponse.unpack(_response);
        auto x = _packedResponse.get_msg();
        auto tt = const_cast<FooResponse *>(x);
        auto xx = static_cast<FooResponse *>(response);
        auto text = *(tt->release_text());
        xx->set_text(text);
        xx->set_result(tt->result());
    };
};

#endif //HOSTSVC_RPCCHANNEL_H
