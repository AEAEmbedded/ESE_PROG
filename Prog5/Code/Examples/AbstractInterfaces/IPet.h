#ifndef IPETCLASS
#define IPETCLASS

#include <string>

using namespace std;

class IPet {
public:
   virtual void gotPetted() = 0;
   virtual string getName() = 0;
   virtual ~IPet() {}
};

#endif