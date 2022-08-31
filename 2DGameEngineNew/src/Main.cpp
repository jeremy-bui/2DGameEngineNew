#include <iostream>
//open the folder to locate where Game.h is
#include "./Game/Game.h"

int main(int argc, char* argv[]) {

    Game game;

    game.Initialize();
    game.Run();
    game.Destroy();

    return 0;
}
