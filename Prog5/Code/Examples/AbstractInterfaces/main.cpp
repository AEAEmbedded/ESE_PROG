#include "PetDog.h"
#include "GoldFish.h"
#include "StrayDog.h"
#include "PoliceDog.h"

int main() {

    PetDog pluto("Pluto");
    pluto.gotPetted();
    pluto.barks();
    
    PoliceDog rex("Rex");
    rex.barks();
    rex.patrol();
    //rex.gotPetted(); // you can't pet Rex, he is not a PetDog after all...

    GoldFish myGoldFish;
    myGoldFish.splashes();
    myGoldFish.gotPetted();
    
    // what about a stray Dog?
    StrayDog stray;
    stray.barks();
}