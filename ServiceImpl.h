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
        pObjectMap _map;
    public:

        EchoServiceImpl() { }

        void setMap(pObjectMap map) {
            _map = map;
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
                    _map->insert({rpcRequest->param(), x});
                    response->set_success(true);

                }
                else if (rpcRequest->method() == "delete") {
                    _map->erase(rpcRequest->param());
                    response->set_success(true);
                }
            }
            else if (p = _map->at((ob))) {
                if (rpcRequest->method() == "add") {
                    p->add(rpcRequest->param());
                    response->set_success(true);
                }
                else if (rpcRequest->method() == "sub") {
                    p->sub(rpcRequest->param());
                    response->set_success(true);
                }
                else if (rpcRequest->method() == "getCount") {
                    auto x = p->getCount();
                    response->set_success(true);
                    response->set_result(x);
                }

            }
        }
    };

    using pService=shared_ptr<EchoServiceImpl>;
}


#endif //HOSTSVC_SERVICEIMPL_H
