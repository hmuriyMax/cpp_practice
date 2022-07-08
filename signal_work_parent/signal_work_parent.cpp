#include <iostream>
#include <Windows.h>
#define uint unsigned int

using namespace std;

char PATH[] = "C:\\prog\\practice\\signal_work_client\\x64\\Debug\\signal_work_client.exe";
char PYTHON_PATH[] = "C:\\Users\\Максим\\AppData\\Local\\Microsoft\\WindowsApps\\python.exe C:\\prog\\practice\\signal_work_python\\main.py";

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
    Beep(fdwCtrlType * 100 + 200, 200);
    cout << "Parent > ";
    switch (fdwCtrlType)
    {
        // Handle the CTRL-C signal.
    case CTRL_C_EVENT:
        cout << "Ctrl-C event";
        return TRUE;

        // CTRL-CLOSE: confirm that the user wants to exit.
    case CTRL_CLOSE_EVENT:
        cout << "Ctrl-Close event";
        return TRUE;

        // Pass other signals to the next handler.
    case CTRL_BREAK_EVENT:
        cout << "Ctrl-Break event";
        return FALSE;

    case CTRL_LOGOFF_EVENT:
        cout << "Ctrl-Logoff event";
        return FALSE;

    case CTRL_SHUTDOWN_EVENT:
        cout << "Ctrl-Shutdown event";
        return FALSE;

    default:
        return FALSE;
    }
    cout << endl;
}

// Моя функция, которая просто возвращает true, как будто событие обработано
BOOL WINAPI EmptyCtrlHandler(DWORD fdwCtrlType) {
    return TRUE;
}

//Отправляет сигнал в дочернее приложение, возвращает наличие ошибки если что-то пошло не так...
static bool SendSignal(DWORD event, uint pid)
{
    bool err_found = false;
    // It's impossible to be attached to 2 consoles at the same time,
    // so release the current one.
    err_found |= FreeConsole();
    // This does not require the console window to be visible.
    // Нельзя ничего в stdout
    if (err_found |= AttachConsole(pid), err_found)
    {
        // Disable Ctrl-C handling for our program
        err_found |= SetConsoleCtrlHandler(EmptyCtrlHandler, true);
        //
        err_found |= GenerateConsoleCtrlEvent(event, 0);

        // Must wait here. If we don't and re-enable Ctrl-C
        // handling below too fast, we might terminate ourselves.
        Sleep(1);

        err_found |= FreeConsole();

        // Re-enable Ctrl-C handling or any subsequently started
        // programs will inherit the disabled state.
    }
    err_found |= AttachConsole(ATTACH_PARENT_PROCESS);
    //ATTACH_PARENT_PROCESS работает, но неясно, почему. GetCurrentProcessId() не работает.
    err_found |= SetConsoleCtrlHandler(EmptyCtrlHandler, false);
    return err_found;
}


int main()
{
    PROCESS_INFORMATION pi = {};
    int input = -1;
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
    cout << "1 to start a process\n2 to send CTRL+C\n3 to send CTRL+BREAK" << endl;
    while (input != 0) {
        DWORD dw;
        cin >> input;
        switch (input) {
        case 0: {
            continue;
            break;
        }
        case 1: {

            if (GetExitCodeProcess(pi.hProcess, &dw), dw == STILL_ACTIVE) {
                cout << "Parent > Child process already exists!" << endl;
                break;
            }
            cout << "Parent > Starting child process..." << endl;

            bool bSuccess = CreateProcessA(NULL, LPSTR(PYTHON_PATH),
                NULL, NULL, FALSE, CREATE_NEW_CONSOLE,
                NULL, NULL, LPSTARTUPINFOA(&si), &pi);

            cout << "Parent > Start status: " << bSuccess << endl;
            if (bSuccess)
                cout << "Parent > Created process with PID = " << GetProcessId(pi.hProcess) << endl;
            else
                cout << "Parent > Error: " << GetLastError() << endl;
            break;
        }

        case 2: {
            if (GetExitCodeProcess(pi.hProcess, &dw), dw != STILL_ACTIVE) {
                cout << "Parent > No child process found" << endl;
                break;
            }

            cout << "Parent > Sending CTRL+C signal..." << endl;
            SendSignal(CTRL_C_EVENT, pi.dwProcessId);
            break;
        }

        case 3: {
            if (GetExitCodeProcess(pi.hProcess, &dw), dw != STILL_ACTIVE) {
                cout << "Parent > No child process found" << endl;
                break;
            }

            cout << "Parent > Sending CTRL+BREAK signal..." << endl;
            SendSignal(CTRL_BREAK_EVENT, pi.dwProcessId);
            break;
        }

        default:
            cout << "Parent > Unknown command" << endl;
        }
    }
    cout << "Parent > finished";
    return 0;
}
