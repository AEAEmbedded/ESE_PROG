#ifndef PETDOG
#define PETDOG

#include <iostream>
#include "IPet.h"
#include "Dog.h"

using namespace std;

class PetDog : public Dog, IPet {
    public:
        void gotPetted();
        PetDog(string name);
        void barks();
    private:
        string getName();
        string _name;
};

void PetDog::barks() {
    cout << "Woof, woof, woof (meaning: I don't bite!)" << endl;
}

void PetDog::gotPetted() {
    cout << _name << " just wagged it's tail to you..." << endl;
}

PetDog::PetDog(string name) {
    _name = name;
}

string PetDog::getName() {
    return _name;
}

#endif
