#ifndef GERMANSHEPHERD
#define GERMANSHEPHERD

#include "Dog.h"
#include <iostream>

using namespace std;

class GermanShepherd : public Dog {
    public:
        GermanShepherd(string name);
        void barks();
    private:
        string _name;
};

#endif