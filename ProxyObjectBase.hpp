//
// Created by lishunan on 4/23/16.
//

#ifndef HOSTSVC_PROXYOBJECTBASE_H
#define HOSTSVC_PROXYOBJECTBASE_H

#include "HostSvcCommon.hpp"
#include "RPCChannel.hpp"
#include "RPCController.hpp"

namespace GkcHostSvc {
    class ProxyObjectBase {
    protected:
        Connection *_rpcChannel;
        EchoService_Stub _service;
        RPCRequest _request;
        RPCResponse _response;
        RPCController _controller;
        int _name;
        string _type;
    public:
        ProxyObjectBase(Connection *rpcChannel, const int name,const string& type) : _rpcChannel(rpcChannel), _service(rpcChannel),
                                                                  _name(name),_type(type)
        {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(0);
            _request.set_method("new");
            _request.set_paramint(_name);
            _request.set_paramstring(_type);
            _service.RPC(&_controller, &_request, &_response, nullptr);
        }

        ProxyObjectBase(const ProxyObjectBase &) = delete;

        ProxyObjectBase &operator=(const ProxyObjectBase &) = delete;

        ProxyObjectBase(ProxyObjectBase &&) = default;

        ProxyObjectBase &operator=(ProxyObjectBase &&) = default;

        virtual ~ProxyObjectBase()
        {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(0);
            _request.set_method("delete");
            _request.set_paramint(_name);

            _service.RPC(&_controller, &_request, &_response, nullptr);
        }
    };
}


#endif //HOSTSVC_PROXYOBJECTBASE_H
