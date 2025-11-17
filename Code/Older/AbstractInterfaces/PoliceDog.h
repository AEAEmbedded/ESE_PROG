#ifndef POLICEDOG
#define POLICEDOG

#include "GermanShepherd.h"
#include <iostream>

using namespace std;

class PoliceDog : public GermanShepherd {
    public:
        PoliceDog(string name);
        void barks();
        void patrol();
    private:
        string _name;
};

#endif