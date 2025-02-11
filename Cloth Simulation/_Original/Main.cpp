#include <iostream>
#include <math.h>
#include "Application.h"
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    Application app;

    app.Setup(1200, 320, 10);

    while(app.IsRunning()){
        app.Input();
        app.Update();
        app.Render();
    }

    app.Destroy();

    return 0;
}