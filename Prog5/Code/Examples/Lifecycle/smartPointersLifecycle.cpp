/*
  smartPointersLifecycle.cpp - Demo of memory leak (>>> avoid this of course! <<<)
  Johan Korten
  V1.0 March 2020

  HAN University of Applied Sciences
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
    
    std::unique_ptr<IamAlife> toBeOrNotToBe(new IamAlife());
    toBeOrNotToBe->shoutAloud();

    std::cout << std::endl;
    std::cout << "Main goes out of scope after this..." << std::endl;
}