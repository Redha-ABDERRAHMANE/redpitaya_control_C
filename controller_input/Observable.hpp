#pragma once
#include "observer.hpp"
template<typename T>
class Observable {
private:
    Observer<T>* observer;


public:
    Observable() {}
    void notifyObserver(T& info){
         observer->update(info); 
    }

    void setObserver(Observer<T>& obs) { observer = obs; }

};
