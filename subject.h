#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>

class Observer;

class Subject {
    std::vector<Observer*> observers;

    public:
    void attach(Observer* o);
    void detach(Observer* o);
    void notifyObservers();
    virtual ~Subject() = 0; // Pure virtual but still needs to be defined. Just make it's body empty.
};

#endif