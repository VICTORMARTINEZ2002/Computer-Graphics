#include <iostream>
#include <math.h>
#include <windows.h>

#include "Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    Application app;

    app.Setup(1200, 320, 10);

    while(app.IsRunning()) 
    {
        app.Input();
        app.Update();
        app.Render();
    }

    app.Destroy();

    return 0;
}