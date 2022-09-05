
//
// Created by jan eckstein on 11.07.22.
//

#include <cstdint>

class NormalClass
{
public:
    virtual void Count(uint64_t n) volatile = 0;
    virtual uint64_t GetValue() volatile = 0;
};


class NormalDerived : public NormalClass
{
public:
    NormalDerived() : counter(0)
    {
    }

    virtual void Count(uint64_t n) volatile
    {
        // counter += n;
        //counter += n % 10;
        counter += n - (n/2 % 5);
    }

    virtual uint64_t GetValue() volatile
    {
        return counter;
    }

private:
    volatile uint64_t counter;
};

void RunNormal(volatile NormalClass* obj)
{
    const unsigned N = 40000;
    for (unsigned i = 0; i < N; ++i)
    {
        for (unsigned j = 0; j < i; ++j)
        {
            obj->Count(j);
        }
    }
};