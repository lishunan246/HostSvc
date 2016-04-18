//
// Created by lishunan on 4/18/16.
//

#ifndef HOSTSVC_PRCCONTROLLER_H
#define HOSTSVC_PRCCONTROLLER_H
#include "HostSvcCommon.h"

class MyRpcController:public protobuf::RpcController
{
private:
    bool _failed=false;
    std::string _errorText="No Error";

public:
    // Client-side methods ---------------------------------------------
    // These calls may be made from the client side only.  Their results
    // are undefined on the server side (may crash).

    // Resets the RpcController to its initial state so that it may be reused in
    // a new call.  Must not be called while an RPC is in progress.
    void Reset() {
        _failed=false;
        _errorText="No Error";
    };

    // After a call has finished, returns true if the call failed.  The possible
    // reasons for failure depend on the RPC implementation.  Failed() must not
    // be called before a call has finished.  If Failed() returns true, the
    // contents of the response message are undefined.
    bool Failed() const
    {
        return _failed;
    };

    // If Failed() is true, returns a human-readable description of the error.
    std::string ErrorText() const
    {
        return _errorText;
    };

    // Advises the RPC system that the caller desires that the RPC call be
    // canceled.  The RPC system may cancel it immediately, may wait awhile and
    // then cancel it, or may not even cancel the call at all.  If the call is
    // canceled, the "done" callback will still be called and the RpcController
    // will indicate that the call failed at that time.
    void StartCancel(){};

    // Server-side methods ---------------------------------------------
    // These calls may be made from the server side only.  Their results
    // are undefined on the client side (may crash).

    // Causes Failed() to return true on the client side.  "reason" will be
    // incorporated into the message returned by ErrorText().  If you find
    // you need to return machine-readable information about failures, you
    // should incorporate it into your response protocol buffer and should
    // NOT call SetFailed().
    void SetFailed(const std::string& reason){
        _failed=true;
        _errorText=reason;
    };

    // If true, indicates that the client canceled the RPC, so the server may
    // as well give up on replying to it.  The server should still call the
    // final "done" callback.
    bool IsCanceled() const
    {
        return false;
    };

    // Asks that the given callback be called when the RPC is canceled.  The
    // callback will always be called exactly once.  If the RPC completes without
    // being canceled, the callback will be called after completion.  If the RPC
    // has already been canceled when NotifyOnCancel() is called, the callback
    // will be called immediately.
    //
    // NotifyOnCancel() must be called no more than once per request.
    void NotifyOnCancel(protobuf::Closure* callback) {};
};


#endif //HOSTSVC_PRCCONTROLLER_H
