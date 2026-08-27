#include <iostream>
#include "Dog.h"

using namespace std;

/*
Impossible because Dog is a virtual function / abstract class...

void Dog::barks() {
    cout << "Woof, woof" << endl;
}*/

Dog::~Dog() {
    cout << "This virtual dog was just put down :(" << endl;
}