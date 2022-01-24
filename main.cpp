#include "CApp.h"

#include <iostream>

// dwa argumenty metody main wymagane przez SDL2
int main(int argc, char* argv[]) {
    CApp theApp;
    return theApp.OnExecute();
}
