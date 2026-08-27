#include <iostream>
#include "Fish.h"

using namespace std;

void Fish::splashes() {
    cout << " made splashes in the water!" << endl;
}

Fish::~Fish() {
    cout << "This virtual fish was just put down :(" << endl;
}
