/*
  betterSmartPointersLifecycle.cpp
  Johan Korten
  V1.0 March 2020
  V1.1 Oct 2023

  HAN University of Applied Sciences

  Note: compile using C++ 14
  (e.g. g++ -std=c++14 ...)
*/

#include <iostream>
#include <vector>
#include <memory>

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
    std::cout << std::endl;

    { // local sub scope
        std::unique_ptr<IamAlife> toBeOrNotToBe = std::make_unique<IamAlife>();
        toBeOrNotToBe->shoutAloud();
    }

    std::cout << std::endl;
    std::cout << "Main goes out of scope after this..." << std::endl;
}

