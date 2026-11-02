#define SDL_MAIN_HANDLED

#include "head.h"
#include "Engine.h"
#include "Player.h"


int main(void)
{      
    //引擎部分
    Engine engine;

    if(!engine.init())
    {
        return -1;
    }

    engine.run();
 
    return 0;
}
