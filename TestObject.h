//
// Created by lishunan on 4/19/16.
//

#ifndef HOSTSVC_TESTOBJECT_H
#define HOSTSVC_TESTOBJECT_H


class TestObject
{
private:
    int _count=0;
public:
    virtual void add(int n)=0;
    virtual void sub(int n)=0;
    virtual int getCount()=0;
};

class RealObject:public TestObject
{

};

class ProxyObject:public TestObject
{
    ProxyObject()
    {

    }
    void add(int n)
    {}
    void sub(int n)
    {}
    int getCount()
    {
        return 0;
    }
    ~ProxyObject()
    {

    }
};


#endif //HOSTSVC_TESTOBJECT_H
