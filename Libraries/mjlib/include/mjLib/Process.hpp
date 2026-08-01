class Process {
private:
    static HANDLE GetToken(DWORD pid) {
        std::string userProcess;
        HANDLE cToken = NULL;
        HANDLE ph = NULL;
        ph = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, true, pid);
        if (ph == NULL) {
            cToken = (HANDLE)NULL;
        }
        else {
            BOOL res = OpenProcessToken(ph, MAXIMUM_ALLOWED, &cToken);
            if (!res) {
                cToken = (HANDLE)NULL;
            }
            else {
            }
        }
        if (ph != NULL) {
            CloseHandle(ph);
        }
        return cToken;
    }

    static BOOL CreateProcess(HANDLE token, LPCWSTR app, LPCWSTR commandLine) {
        HANDLE dToken = NULL;
        STARTUPINFOW si;
        PROCESS_INFORMATION pi;
        BOOL res = TRUE;
        ZeroMemory(&si, sizeof(STARTUPINFOW));
        ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
        si.cb = sizeof(STARTUPINFOW);
        res = DuplicateTokenEx(token, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenPrimary, &dToken);
        if (!res) {
            return FALSE;
        }

        size_t commandLineLength = wcslen(commandLine) + 1;
        LPWSTR modifiableCommandLine = new WCHAR[commandLineLength];
        wcscpy_s(modifiableCommandLine, commandLineLength, commandLine);
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;
        res = CreateProcessWithTokenW(dToken, LOGON_WITH_PROFILE, app, modifiableCommandLine, 0, NULL, NULL, &si, &pi);
        if (!res) {
            delete[] modifiableCommandLine;
            return FALSE;
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(dToken);
        delete[] modifiableCommandLine;
        return TRUE;
    }

    static std::string GetProcessUserName(DWORD pid) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (!hProcess) return "";

        HANDLE hToken = NULL;
        if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
            CloseHandle(hProcess);
            return "";
        }

        DWORD dwSize = 0;
        GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize);
        PTOKEN_USER pTokenUser = (PTOKEN_USER)malloc(dwSize);
        SID_NAME_USE SidType;
        char lpName[MAX_PATH];
        DWORD dwNameSize = MAX_PATH;
        char lpDomain[MAX_PATH];
        DWORD dwDomainSize = MAX_PATH;
        if (!LookupAccountSid(NULL, pTokenUser->User.Sid, lpName, &dwNameSize, lpDomain, &dwDomainSize, &SidType)) {
            free(pTokenUser);
            CloseHandle(hToken);
            CloseHandle(hProcess);
            return "";
        }

        std::string username(lpDomain);
        username += "/";
        username += lpName;
        free(pTokenUser);
        CloseHandle(hToken);
        CloseHandle(hProcess);
        return username;
    }
public:
    static DWORD GetId(const std::string& processName) {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            return 0;
        }

        if (!Process32First(hSnapshot, &pe32)) {
            CloseHandle(hSnapshot);
            return 0;
        }

        do {
            if (processName == pe32.szExeFile) {
                CloseHandle(hSnapshot);
                return pe32.th32ProcessID;
            }
        } while (Process32Next(hSnapshot, &pe32));

        CloseHandle(hSnapshot);
        return 0;
    }

    static std::string GetName(DWORD pID) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pID);
        if (hProcess == nullptr) {
            return "";
        }

        char processName[MAX_PATH];
        DWORD dwSize = MAX_PATH;
        if (QueryFullProcessImageNameA(hProcess, 0, processName, &dwSize)) {
            std::string fullPath(processName);
            size_t pos = fullPath.find_last_of("\\/");
            if (pos != std::string::npos) {
                fullPath = fullPath.substr(pos + 1);
            }
            CloseHandle(hProcess);
            return fullPath;
        }
        CloseHandle(hProcess);
        return "";
    }

    static DWORD64 GetModuleAddress(DWORD processID, const std::string& moduleName) {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            return 0;
        }
        MODULEENTRY32 moduleEntry = { 0 };
        moduleEntry.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(hSnapshot, &moduleEntry)) {
            do {
                if (moduleName == moduleEntry.szModule) {
                    CloseHandle(hSnapshot);
                    return (DWORD64)moduleEntry.modBaseAddr;
                }
            } while (Module32Next(hSnapshot, &moduleEntry));
        }
        CloseHandle(hSnapshot);
        return 0;
    }

    static std::string GetFilePath(DWORD processId) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
        if (hProcess == NULL) {
            return "";
        }

        char filePath[MAX_PATH];
        if (GetModuleFileNameEx(hProcess, NULL, filePath, sizeof(filePath)) == 0) {
            CloseHandle(hProcess);
            return "";
        }

        CloseHandle(hProcess);
        return std::string(filePath);
    }

    static std::string GetFilePath(const std::string& processName) {
        return GetFilePath(GetId(processName));
    }

    static bool Check(DWORD processId) {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            return false;
        }

        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe)) {
            do {
                if (pe.th32ProcessID == processId) {
                    CloseHandle(hSnapshot);
                    return true;
                }
            } while (Process32Next(hSnapshot, &pe));
        }

        CloseHandle(hSnapshot);
        return false;
    }

    static bool Check(const std::string& processName) {
        if (GetId(processName) != 0) {
            return true;
        }
        return false;
    }

    static bool Kill(DWORD processId) {
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
        if (hProcess == NULL) {
            return false;
        }
        BOOL result = TerminateProcess(hProcess, 0);
        if (result == 0) {
            CloseHandle(hProcess);
            return false;
        }
        CloseHandle(hProcess);
        return true;
    }

    static std::string GetParent(DWORD pid) {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            return "";
        }

        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe32)) {
            do {
                if (pe32.th32ProcessID == pid) {
                    DWORD parentPID = pe32.th32ParentProcessID;
                    if (Process32First(hSnapshot, &pe32)) {
                        do {
                            if (pe32.th32ProcessID == parentPID) {
                                CloseHandle(hSnapshot);
                                return pe32.szExeFile;
                            }
                        } while (Process32Next(hSnapshot, &pe32));
                    }
                }
            } while (Process32Next(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
        return "";
    }

    static bool Kill(const std::string processName) {
        return Kill(GetId(processName));
    }

    static std::string ReadString(HANDLE hProcess, DWORD64 address) {
        char buffer[256] = { 0 };
        SIZE_T bytesRead;
        if (ReadProcessMemory(hProcess, (LPCVOID)address, buffer, sizeof(buffer), &bytesRead)) {
            return std::string(buffer);
        }
        else {
            return "";
        }
    }

    static int StartApp(std::string path, LPCWSTR commandLine) {
        std::string username;
        HANDLE hProcSnap;
        PROCESSENTRY32 pe32;
        std::string userProcess;
        int pid = 0;

        hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        pe32.dwSize = sizeof(PROCESSENTRY32);
        std::wstring wapp = std::wstring(path.begin(), path.end());
        LPCWSTR LPCapp = wapp.c_str();

        if (!Process32First(hProcSnap, &pe32)) {
            CloseHandle(hProcSnap);
            return 0;
        }

        while (Process32Next(hProcSnap, &pe32)) {
            pid = pe32.th32ProcessID;
            username = GetProcessUserName(pid);
            if (username == "" || username == "NT AUTHORITY/SYSTEM") {
                bool success = false;
                HANDLE cToken = GetToken(pid);
                if (cToken != NULL || cToken == 0) {
                    success = CreateProcess(cToken, LPCapp, commandLine);
                    if (success) {
                        break;
                    }
                }
            }
        }
        CloseHandle(hProcSnap);
        return 0;
    }
};