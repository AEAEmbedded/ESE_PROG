/*
  moreComplexLifecycle.cpp - Demo of memory leak (>>> avoid this of course! <<<)
  Johan Korten
  V1.0 March 2020

  HAN University of Applied Sciences
*/

#include <iostream>
#include <vector>

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

std::vector<IamAlife *> _hamlets;

int main() {
    std::cout << "There we go..." << std::endl;

    IamAlife *toBeOrNotToBe = new IamAlife(); // be careful, new so also call destructor before object goes out of scope
    toBeOrNotToBe->shoutAloud();

    std::cout << std::endl;
    std::cout << "Main goes out of scope after this..." << std::endl;
    std::cout << "Whoops IamAlife is about to become a zombie..." << std::endl;
    delete toBeOrNotToBe; // phew... just in time...
}