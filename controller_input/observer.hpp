#pragma once

template<typename T>

class Observer
{
private:
    /* data */
public:
    virtual void update(const T& info) = 0;

};
