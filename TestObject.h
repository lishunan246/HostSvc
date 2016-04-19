//
// Created by lishunan on 4/19/16.
//

#ifndef HOSTSVC_TESTOBJECT_H
#define HOSTSVC_TESTOBJECT_H

#include "RPCChannel.h"
#include "RPCController.h"
class TestObject
{
public:
    virtual void add(int n)=0;
    virtual void sub(int n)=0;
    virtual int getCount()=0;
};

class RealObject: public TestObject
{
private:
    int _count=0;
public:
    void add(int n)
    {
        _count+=n;
    }
    void sub(int n)
    {
        _count-=n;
    }
    int getCount()
    {
        return _count;
    }
};

class ProxyObject:public TestObject
{
    Connection* _rpcChannel;
    EchoService_Stub echo_clt;
    RPCRequest request;
    RPCResponse response;
    RPCController controller;
    int _name;
public:
    ProxyObject(Connection* rpcChannel,const int name):_rpcChannel(rpcChannel),echo_clt(rpcChannel),_name(name)
    {
        request.set_clientid(_rpcChannel->getClientID());
        request.set_object(0);
        request.set_method("new");
        request.set_param(_name);

        echo_clt.RPC(&controller, &request, &response, nullptr);

    }
    void add(int n)
    {
        request.set_clientid(_rpcChannel->getClientID());
        request.set_object(_name);
        request.set_method("add");
        request.set_param(n);

        echo_clt.RPC(&controller, &request, &response, nullptr);

    }
    void sub(int n)
    {
        request.set_clientid(_rpcChannel->getClientID());
        request.set_object(_name);
        request.set_method("sub");
        request.set_param(n);

        echo_clt.RPC(&controller, &request, &response, nullptr);

    }
    int getCount()
    {
        request.set_clientid(_rpcChannel->getClientID());
        request.set_object(_name);
        request.set_method("getCount");
        request.set_param(0);

        echo_clt.RPC(&controller, &request, &response, nullptr);

        return response.result();
    }
    ~ProxyObject()
    {
        request.set_clientid(_rpcChannel->getClientID());
        request.set_object(0);
        request.set_method("delete");
        request.set_param(_name);

        echo_clt.RPC(&controller, &request, &response, nullptr);
    }
};


#endif //HOSTSVC_TESTOBJECT_H
