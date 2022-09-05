//
// Created by jan eckstein on 11.07.22.
//

#include <cstdint>

template <typename Derived>
class CRTPClass
{
public:
    void Count(uint64_t n) volatile
    {
        static_cast<volatile Derived*>(this)->Count(n);
    }

    uint64_t GetValue() volatile
    {
        return static_cast<Derived*>(this)->GetValue();
    }
};

class CRTPDerived : public CRTPClass<CRTPDerived>
{
private:
    volatile uint64_t counter;
public:
    CRTPDerived() : counter(0)
    {
    }

    void Count(uint64_t n) volatile
    {
        // counter += n;
        //counter += n % 10;
        counter += n - (n/2 % 5);
    }

    uint64_t GetValue() volatile
    {
        return counter;
    }
};

template <typename T>
void RunCRTP(volatile CRTPClass<T>* obj)
{
    const unsigned N = 40000;
    for (unsigned i = 0; i < N; ++i)
    {
        for (unsigned j = 0; j < i; ++j)
        {
            obj->Count(j);
        }
    }
}