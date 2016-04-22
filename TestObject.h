//
// Created by lishunan on 4/19/16.
//

#ifndef HOSTSVC_TESTOBJECT_H
#define HOSTSVC_TESTOBJECT_H

#include "RPCChannel.h"
#include "RPCController.h"

namespace GkcHostSvc {
    class TestObject {
    public:
        virtual void add(int n) = 0;

        virtual void sub(int n) = 0;

        virtual int getCount() = 0;
    };

    class RealObject : public TestObject {
    private:
        int _count = 0;
    public:
        void add(int n) {
            _count += n;
        }

        void sub(int n) {
            _count -= n;
        }

        int getCount() {
            return _count;
        }
    };

    class ProxyObject : public TestObject {
    private:
        Connection *_rpcChannel;
        EchoService_Stub _service;
        RPCRequest _request;
        RPCResponse _response;
        RPCController _controller;
        int _name;
    public:
        ProxyObject(Connection *rpcChannel, const int name) : _rpcChannel(rpcChannel), _service(rpcChannel),
                                                              _name(name) {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(0);
            _request.set_method("new");
            _request.set_param(_name);

            _service.RPC(&_controller, &_request, &_response, nullptr);

        }

        void add(int n) {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(_name);
            _request.set_method("add");
            _request.set_param(n);

            _service.RPC(&_controller, &_request, &_response, nullptr);

        }

        void sub(int n) {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(_name);
            _request.set_method("sub");
            _request.set_param(n);

            _service.RPC(&_controller, &_request, &_response, nullptr);

        }

        int getCount() {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(_name);
            _request.set_method("getCount");
            _request.set_param(0);

            _service.RPC(&_controller, &_request, &_response, nullptr);

            return _response.result();
        }

        ~ProxyObject() {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(0);
            _request.set_method("delete");
            _request.set_param(_name);

            _service.RPC(&_controller, &_request, &_response, nullptr);
        }
    };

    using ObjectMap=std::map<int, shared_ptr<RealObject>>;
    using pObjectMap=std::shared_ptr<ObjectMap>;
}


#endif //HOSTSVC_TESTOBJECT_H
