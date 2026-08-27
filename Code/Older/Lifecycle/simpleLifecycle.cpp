/*
  simpleLifecycle.cpp - Demo of scope and lifetime of objects
  Johan Korten
  V1.0 March 2020

  HAN University of Applied Sciences
*/
#include <iostream>

class IamAlife {

public:
    IamAlife() {
        std::cout << "I was created ex nihilo!" << std::endl;
    }

    ~IamAlife() {
        std::cout << "My lifecycle ended: IamAlife is no more :(" << std::endl;
    }

    void shoutAloud() {
        std::cout << "Whoohoo, IamAlife!" << std::endl;
    }
};

void otherMethodScope();

int main() {
    std::cout << "There we go..." << std::endl;
    IamAlife toBeOrNotToBe;
    toBeOrNotToBe.shoutAloud();
    
    std::cout << std::endl;

    // to see more how scope works (e.g. a for loop) 
    for (int i = 0; i < 3; i++) {
        std::cout << i << std::endl;
        IamAlife toBeOrNotToBe;
        toBeOrNotToBe.shoutAloud();
    }

    {
        IamAlife toBeOrNotToBe;
        toBeOrNotToBe.shoutAloud();
    }

    std::cout << std::endl;
    otherMethodScope();

    std::cout << std::endl;
    std::cout << "Main goes out of scope after this..." << std::endl;
    
}

void otherMethodScope() {
    IamAlife toBeOrNotToBe;
    toBeOrNotToBe.shoutAloud();
    std::cout << "otherMethodScope goes out of scope after this..." << std::endl;
}