//
// Created by lishunan on 4/23/16.
//

#ifndef HOSTSVC_REALOBJECTBASE_H
#define HOSTSVC_REALOBJECTBASE_H

#include <string>
#include "HostSvcCommon.hpp"

namespace GkcHostSvc {
    class RealObjectBase {
    protected:
        string _type;
    public:
        RealObjectBase() { }

        RealObjectBase(const RealObjectBase &) = delete;

        RealObjectBase &operator=(const RealObjectBase &) = delete;

        RealObjectBase(RealObjectBase &&) = default;

        RealObjectBase &operator=(RealObjectBase &&) = default;

        virtual ~RealObjectBase() { }

        const string &get_type() {
            return _type;
        }
    };
}


#endif //HOSTSVC_REALOBJECTBASE_H
