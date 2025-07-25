#ifndef SUBJECT_H
#define SUBJECT_H

#include "observer.h"
#include <vector>

class Observer;

class Subject {
protected:
    std::vector<Observer *> observers;

public:
    void attach(Observer* o);
    void detach(Observer* o);
    void notifyObservers();

    // Pure virtual but still needs to be defined. Just make it's body empty.
    virtual ~Subject() = 0;
};

#endif
