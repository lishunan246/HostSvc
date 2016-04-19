//
// Created by lishunan on 4/20/16.
//

#ifndef HOSTSVC_SERVICEIMPL_H
#define HOSTSVC_SERVICEIMPL_H

#include "HostSvcCommon.h"
#include "TestObject.h"
#include "RPCController.h"

namespace GkcHostSvc {
    class EchoServiceImpl : public EchoService {
        std::map<int, shared_ptr<RealObject>> ObjectMap;

    public:

        EchoServiceImpl() { }

        virtual void Foo(::google::protobuf::RpcController *controller,

                         const ::FooRequest *request,

                         ::FooResponse *response,

                         ::google::protobuf::Closure *done) {
            std::cout << " str:" << std::endl;
            std::string str = request->text();
            std::cout << " str:" << str << std::endl;

            std::string tmp = str;

            for (int i = 1; i < request->times(); i++)

                str += (" " + tmp);

            response->set_text(str);

            response->set_result(true);

        }

        virtual void RPC(RPCController *controller, const RPCRequest *rpcRequest, RPCResponse *response,
                         ::protobuf::Closure *done) {
            auto ob = rpcRequest->object();
            auto clinetid = rpcRequest->clientid();
            response->set_clientid(clinetid);
            std::shared_ptr<RealObject> p;
            if (ob == 0) {
                if (rpcRequest->method() == "new") {

                        auto x = make_shared<RealObject>();
                        ObjectMap[rpcRequest->param()] = x;
                        response->set_success(true);

                }
            }
            else if(p=ObjectMap[ob])
            {
                if(rpcRequest->method()=="add")
                {
                    p->add(rpcRequest->param());
                    response->set_success(true);
                }
                else if(rpcRequest->method()=="sub")
                {
                    p->sub(rpcRequest->param());
                    response->set_success(false);
                }
                else if(rpcRequest->method()=="getCount")
                {
                    auto x=p->getCount();
                    response->set_success(true);
                    response->set_result(x);
                }

            }
        }
    };
}


#endif //HOSTSVC_SERVICEIMPL_H
