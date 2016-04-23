//
// Created by lishunan on 4/20/16.
//

#ifndef HOSTSVC_SERVICEIMPL_H
#define HOSTSVC_SERVICEIMPL_H

#include "HostSvcCommon.hpp"
#include "TestObject.h"
#include "RPCController.hpp"

namespace GkcHostSvc {
    class EchoServiceImpl : public EchoService {
        pObjectMap _map;
    public:

        EchoServiceImpl() { }
        EchoServiceImpl(const EchoServiceImpl&)= delete;
        EchoServiceImpl(EchoServiceImpl&&)=default;
        EchoServiceImpl&operator=(EchoServiceImpl&&)= default;
        EchoServiceImpl&operator=(const EchoServiceImpl&)= delete;

        void setMap(pObjectMap map) {
            _map = map;
        }

        virtual void RPC(RPCController *controller, const RPCRequest *rpcRequest, RPCResponse *response,
                         ::protobuf::Closure *done) {
            auto ob = rpcRequest->object();
            response->set_success(false);
            std::shared_ptr<RealObjectBase> p;
            if (ob == 0) {
                if (rpcRequest->method() == "new") {
                    if (rpcRequest->paramstring() == "counter") {
                        auto x = make_shared<RealObject>();
                        _map->insert({rpcRequest->paramint(), x});
                        response->set_success(true);

                    }
                    else if (rpcRequest->paramstring() == "string") {
                        auto x = make_shared<RealStringObject>();
                        _map->insert({rpcRequest->paramint(), x});
                        response->set_success(true);
                    }
                }
                else if (rpcRequest->method() == "delete") {
                    _map->erase(rpcRequest->paramint());
                    response->set_success(true);
                }
            }
            else if (p = _map->at(ob)) {
                if (p->get_type() == "counter") {
                    auto xp = dynamic_pointer_cast<RealObject>(p);
                    if (rpcRequest->method() == "add") {
                        xp->add(rpcRequest->paramint());
                        response->set_success(true);
                    }
                    else if (rpcRequest->method() == "sub") {
                        xp->sub(rpcRequest->paramint());
                        response->set_success(true);
                    }
                    else if (rpcRequest->method() == "getCount") {
                        auto x = xp->getCount();
                        response->set_success(true);
                        response->set_resultint(x);
                    }
                }
                else if (p->get_type() == "string") {
                    auto xp = dynamic_pointer_cast<RealStringObject>(p);
                    auto method = rpcRequest->method();
                    if (method == "append") {
                        xp->append(rpcRequest->paramstring());
                        response->set_success(true);
                    }
                    if (method == "clear") {
                        xp->clear();
                        response->set_success(true);
                    }
                    if (method == "get") {
                        auto s = xp->get();
                        response->set_resultstring(s);
                        response->set_success(true);
                    }
                }
            }
        }
    };
    using pService=shared_ptr<EchoServiceImpl>;
}


#endif //HOSTSVC_SERVICEIMPL_H
