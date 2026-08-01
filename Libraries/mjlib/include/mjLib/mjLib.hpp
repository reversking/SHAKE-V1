#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <random>
#include <dwmapi.h>
#include <thread>
#include <regex>
#include <dxgi.h>
#include <winternl.h>
#include <gdiplus.h>
#include <unordered_map>
#include <exception>
#include <functional>
#include <typeinfo>
#include <XorStr/XorStr.hpp>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4244)

namespace mjLib {
#include "String.hpp"
#include "Process.hpp"
}

void RunThread(std::function<void()> func, const char* functionName, int lineNumber, const std::string& threadName = XorString("Worker")) {
    std::thread([func, functionName, lineNumber, threadName]() {
        try {
            func();
        }
        catch (const std::exception& e) {
            (void)e;
        }
        catch (...) {
        }
    }).detach();
}

#define RUN_THREAD(func, threadName) RunThread(func, __func__, __LINE__, threadName)
