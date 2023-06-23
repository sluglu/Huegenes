#include <GLContext.h>
#include <random>


int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, char* szCmdLine, int iCmdShow) {
    GLContext::window_name = "Huegene";
    GLContext::init(1500, 1000);
    return 0;
}


