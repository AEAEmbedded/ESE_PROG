#include <iostream>
#include <typeinfo>

#include "IPet.h"
#include "Fish.h"

using namespace std;

class GoldFish : public Fish, IPet {
    public:
        void gotPetted();
        void splashes();
    private:
        string getName();
};

void GoldFish::gotPetted() {
    cout << this->getName() << " just leaped out of the water!" << endl;
}

string GoldFish::getName() {
    return typeid(this).name();
}

void GoldFish::splashes() {
     cout << this->getName();
     Fish::splashes();
}