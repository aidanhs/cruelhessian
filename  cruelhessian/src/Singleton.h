#ifndef SINGLETON_H
#define SINGLETON_H

#include <cassert>

template <typename T> class Singleton
{
    static T* ms_Singleton;

public:
    Singleton(void)
    {
        assert(!ms_Singleton);
        long offset = (long)(T*)1 - (long)(Singleton <T>*)(T*)1;
        ms_Singleton = (T*)((long)this + offset);
    }

    virtual ~Singleton(void)
    {
        assert(ms_Singleton);
        ms_Singleton = 0;
    }
    static T& GetSingleton(void)
    {
        assert(ms_Singleton);
        return (*ms_Singleton);
    }
    static T* GetSingletonPtr(void)
    {
        return (ms_Singleton);
    }
};

template <typename T> T* Singleton <T>::ms_Singleton = 0;


#endif
