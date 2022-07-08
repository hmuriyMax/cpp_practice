#include <iostream>
#include <Windows.h>

using namespace std;

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
    Beep(fdwCtrlType * 100 + 200, 200);
    cout << "Child  > ";
    switch (fdwCtrlType)
    {
    case CTRL_C_EVENT:
        cout << "Ctrl-C event";
        break;

    case CTRL_BREAK_EVENT:
        cout << "Ctrl-Break event" << endl;
        cout << "Child  > Im cleaning mess around me..." << endl;
        Sleep(2000);
        cout << "Child  > Good bye!" << endl;
        return FALSE;

    case CTRL_CLOSE_EVENT:
        cout << "Ctrl-Close event";
        break;

    case CTRL_LOGOFF_EVENT:
        cout << "Ctrl-Logoff event";
        break;

    case CTRL_SHUTDOWN_EVENT:
        cout << "Ctrl-Shutdown event";
        break;

    default:
        break;
    }
    cout << endl;
    return TRUE;
}

int main() {
    cout << "Child  > started" << endl;
    int i = 0;
    SetConsoleCtrlHandler(CtrlHandler, TRUE);

    while (true) {
    }

    cout << "Child  > finished";
    return 0;
}