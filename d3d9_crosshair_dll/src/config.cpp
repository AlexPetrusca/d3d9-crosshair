#include "pch.h"
#include "config.h"

#include <iostream>

HKEY saveKey;

void GetExeName(char (&buffer)[MAX_PATH]) {
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    char* exeName = strrchr(buffer, '\\') + 1;
    *strrchr(exeName, '.') = '\0';

    const int n = (int)strlen(exeName);
    for (int i = 0; i < n; i++) {
        buffer[i] = exeName[i];
    }
    buffer[n] = '\0';
}

void GetSaveKeyName(char (&buffer)[MAX_PATH]) {
    char exeName[MAX_PATH];
    GetExeName(exeName);
    buffer[0] = '\0';
    strcat_s(buffer, "Software\\D3D9Crosshair\\");
    strcat_s(buffer, exeName);
}

HKEY OpenKey(HKEY hRootKey, LPCSTR strKey) {
    HKEY hKey;
    LONG nError = RegOpenKeyExA(hRootKey, strKey, 0, KEY_ALL_ACCESS, &hKey);

    if (nError == ERROR_FILE_NOT_FOUND) {
        std::cout << "Creating registry key: " << strKey << std::endl;
        nError = RegCreateKeyExA(hRootKey, strKey, 0, nullptr, REG_OPTION_NON_VOLATILE,
            KEY_ALL_ACCESS,nullptr, &hKey, nullptr);
    }

    if (nError) {
        std::cout << "Error: " << nError << " Could not find or create " << strKey << std::endl;
    }

    return hKey;
}

void SetIntVal(HKEY hKey, LPCSTR lpValue, DWORD data) {
    LONG nError = RegSetValueExA(hKey, lpValue, 0, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));
    
    if (nError) {
        std::cout << "Error: " << nError << " Could not set registry value: " << (char*)lpValue << std::endl;
    }
}

DWORD GetIntVal(HKEY hKey, LPCSTR lpValue) {
    DWORD data;
    DWORD size = sizeof(data);
    DWORD type = REG_DWORD;
    LONG nError = RegQueryValueExA(hKey, lpValue, nullptr, &type, (LPBYTE)&data, &size);

    if (nError == ERROR_FILE_NOT_FOUND) {
        data = -1;
    } else if (nError) {
        std::cout << "Error: " << nError << " Could not get registry value " << (char*)lpValue << std::endl;
    }

    return data;
}

void InitializeConfig() {
    char keyName[MAX_PATH];
    GetSaveKeyName(keyName);
    saveKey = OpenKey(HKEY_CURRENT_USER, keyName);
}

void SetIntConfig(LPCSTR name, DWORD value) {
    SetIntVal(saveKey, name, value);
}

int GetIntConfig(LPCSTR name) {
    return GetIntVal(saveKey, name);
}
