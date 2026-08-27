#ifndef STRAYDOG
#define STRAYDOG

#include "Dog.h"
#include <iostream>

using namespace std;

class StrayDog : public Dog {
    public:
        void barks() override;
    private:
};

void StrayDog::barks() {
    cout << "Some stray dog: I always bark in fear of the dog catcher!" << endl;
}

#endif