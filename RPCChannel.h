//
// Created by lishunan on 4/19/16.
//

#ifndef HOSTSVC_RPCCHANNEL_H
#define HOSTSVC_RPCCHANNEL_H

#include "HostSvcCommon.h"

using namespace GkcHostSvc;

class MyRpcChannel : public protobuf::RpcChannel {
private:
    asio::io_service io_service;
    tcp::resolver resolver;
    tcp::resolver::query query;
    tcp::resolver::iterator endpoint_iterator;
    tcp::socket _soc;
    //p_socket _socket;
    std::vector<char> _request, _response;
    PackedMessage<FooRequest> pm;
    PackedMessage<FooResponse> pr;
public:
    MyRpcChannel(const string& server,const string& PORT) :resolver(io_service),query(server, PORT),endpoint_iterator(resolver.resolve(query)),_soc(io_service)
    {
        asio::connect(_soc, endpoint_iterator);
    }

    ~MyRpcChannel() { }

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
        pm.set_msg(p);
        pm.pack(_request);

        asio::error_code ec;
        asio::write(_soc,asio::buffer(_request), ec);
        done->Run();
        _response.resize(HEADER_SIZE);
        asio::read(_soc, asio::buffer(_response), ec);

        auto size = pr.decode_header(_response);
        _response.resize(size + HEADER_SIZE);
        asio::read(_soc, asio::buffer(&_response[HEADER_SIZE], size), ec);
        pr.unpack(_response);
        auto x = pr.get_msg();
        auto tt = const_cast<FooResponse *>(x);
        auto xx = static_cast<FooResponse *>(response);
        auto text = *(tt->release_text());
        xx->set_text(text);
        xx->set_result(tt->result());
    };
};

#endif //HOSTSVC_RPCCHANNEL_H
