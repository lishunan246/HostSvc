//
// Created by lishunan on 4/19/16.
//

#ifndef HOSTSVC_TESTOBJECT_H
#define HOSTSVC_TESTOBJECT_H

#include "RPCChannel.hpp"
#include "RPCController.hpp"
#include "RealObjectBase.hpp"
#include "ProxyObjectBase.hpp"

namespace GkcHostSvc {


    class TestStringObject {
    protected:
        static const string _type;
    public:
        virtual void append(const std::string &s) = 0;

        virtual void clear() = 0;

        virtual const std::string &get() = 0;
    };

    const string  TestStringObject::_type = "string";

    class RealStringObject : public TestStringObject, public RealObjectBase {
    private:
        std::string _string;

    public:
        RealStringObject() {
            RealObjectBase::_type = TestStringObject::_type;
        }

        void append(const string &s) override {
            _string += s;
        }

        void clear() override {
            _string = "";
        }

        const string &get() override {
            return _string;
        }
    };

    class ProxyStringObject : public TestStringObject, public ProxyObjectBase {
    public:
        ProxyStringObject(Connection *rpcChannel, const int name) : ProxyObjectBase(rpcChannel, name,
                                                                                    TestStringObject::_type) {}

        void append(const string &s) override {
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

        const string &get() override {
            _request.set_clientid(_rpcChannel->getClientID());
            _request.set_object(_name);
            _request.set_method("get");
            _request.set_paramint(0);

            _service.RPC(&_controller, &_request, &_response, nullptr);

            return _response.resultstring();
        }
    };


    class TestObject {
    protected:
        const static string _type;
    public:
        virtual void add(int n) = 0;

        virtual void sub(int n) = 0;

        virtual int getCount() = 0;
    };

    const string TestObject::_type = "counter";

    class RealObject : public TestObject, public RealObjectBase {
    private:
        int _count = 0;
    public:
        RealObject() {
            RealObjectBase::_type = TestObject::_type;
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

    class ProxyObject : public TestObject, public ProxyObjectBase {

    public:
        ProxyObject(Connection *rpcChannel, const int name) : ProxyObjectBase(rpcChannel, name, TestObject::_type) {}

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


    };

    using ObjectMap=std::map<int, shared_ptr<RealObjectBase>>;
    using pObjectMap=std::shared_ptr<ObjectMap>;
}


#endif //HOSTSVC_TESTOBJECT_H
