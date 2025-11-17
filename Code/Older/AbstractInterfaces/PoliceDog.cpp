#include "PoliceDog.h"
#include <iostream>

using namespace std;

PoliceDog::PoliceDog(string name) : GermanShepherd(name) {
    _name = name;
}

void PoliceDog::barks() {
    cout << _name << " barks loudly!" << endl;
}

void PoliceDog::patrol() {
    cout << _name << " is going on patrol!" << endl;
}