#ifndef OBSERVABLE_HPP
#define OBSERVABLE_HPP

#include "observer.hpp"

// Fix 1: Observable.hpp - Add null pointer check
template<typename T>
class Observable {
private:
    Observer<T>* observer = nullptr;  // Initialize to nullptr

public:
    Observable() {}
    void notifyObserver(T& info) {
        if (observer) {  // Add null check
            observer->update(info);
        }
    }

    void setObserver(Observer<T>& obs) { observer = &obs; }  // Take address
};

#endif // OBSERVABLE_HPP