//
// Created by lishunan on 4/19/16.
//

#ifndef HOSTSVC_TESTOBJECT_H
#define HOSTSVC_TESTOBJECT_H

#include "RPCChannel.h"
#include "RPCController.h"

namespace GkcHostSvc
{
    class RealObjectBase
    {
    private:
        string _type;
    public:
        virtual ~RealObjectBase() {}
        const string &get_type() const {
            return _type;
        }

        void set_type(const string &_type) {
            RealObjectBase::_type = _type;
        }
    };


    class TestStringObject
    {
    public:
        virtual void append(const std::string& s)=0;
        virtual void clear()=0;
        virtual const std::string& get()=0;
    };

    class RealStringObject:public TestStringObject,public RealObjectBase
    {
    private:
        std::string _string;

    public:
        RealStringObject()
        {
            set_type("string");
        }
        void append(const string& s) override
        {
            _string+=s;
        }
        void clear() override
        {
            _string="";
        }
        const string& get() override
        {
            return _string;
        }
    };

    class ProxyStringObject:public TestStringObject
    {
    private:
        Connection *_rpcChannel;
        EchoService_Stub _service;
        RPCRequest _request;
        RPCResponse _response;
        RPCController _controller;
        int _name;
    public:
        ProxyStringObject(Connection *rpcChannel, const int name) : _rpcChannel(rpcChannel), _service(rpcChannel),
        _name(name) {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(0);
            _request.set_method("new");
            _request.set_paramint(_name);
            _request.set_paramstring("string");
            _service.RPC(&_controller, &_request, &_response, nullptr);

        }

        void append(const string& s) override {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(_name);
            _request.set_method("append");
            _request.set_paramstring(s);

            _service.RPC(&_controller, &_request, &_response, nullptr);

        }

        void clear() override {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(_name);
            _request.set_method("clear");
            _service.RPC(&_controller, &_request, &_response, nullptr);

        }

        const string& get() override {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(_name);
            _request.set_method("get");
            _request.set_paramint(0);

            _service.RPC(&_controller, &_request, &_response, nullptr);

            return _response.resultstring();
        }

        ~ProxyStringObject()
        {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(0);
            _request.set_method("delete");
            _request.set_paramint(_name);

            _service.RPC(&_controller, &_request, &_response, nullptr);
        }
    };



    class TestObject {
    public:
        virtual void add(int n) = 0;

        virtual void sub(int n) = 0;

        virtual int getCount() = 0;
    };

    class RealObject : public TestObject, public RealObjectBase
    {
    private:
        int _count = 0;
    public:
        RealObject()
        {
            set_type("counter");
        }

        void add(int n) override {
            _count += n;
        }

        void sub(int n) override {
            _count -= n;
        }

        int getCount() override {
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
            _request.set_paramint(_name);
            _request.set_paramstring("counter");

            _service.RPC(&_controller, &_request, &_response, nullptr);

        }

        void add(int n) override {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(_name);
            _request.set_method("add");
            _request.set_paramint(n);

            _service.RPC(&_controller, &_request, &_response, nullptr);

        }

        void sub(int n) override {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(_name);
            _request.set_method("sub");
            _request.set_paramint(n);

            _service.RPC(&_controller, &_request, &_response, nullptr);

        }

        int getCount() override {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(_name);
            _request.set_method("getCount");
            _request.set_paramint(0);

            _service.RPC(&_controller, &_request, &_response, nullptr);

            return _response.resultint();
        }

        ~ProxyObject() {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(0);
            _request.set_method("delete");
            _request.set_paramint(_name);

            _service.RPC(&_controller, &_request, &_response, nullptr);
        }
    };

    using ObjectMap=std::map<int, shared_ptr<RealObjectBase>>;
    using pObjectMap=std::shared_ptr<ObjectMap>;
}


#endif //HOSTSVC_TESTOBJECT_H
