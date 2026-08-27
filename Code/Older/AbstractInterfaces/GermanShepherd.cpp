#include "GermanShepherd.h"
#include <iostream>

GermanShepherd::GermanShepherd(string name) {
    _name = name;
}

void GermanShepherd::barks() {
    cout << _name << " barks loudly!" << endl;
}