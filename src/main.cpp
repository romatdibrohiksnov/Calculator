#include "ui/Application.hpp"

int main(int argc, char *argv[]) {
    Application app;
    if (app.initialize()) {
        app.run();
    }
    return 0;
}
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    return main(__argc, __argv);
}
#endif
