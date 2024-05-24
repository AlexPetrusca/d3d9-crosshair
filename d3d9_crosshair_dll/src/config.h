#pragma once

void InitializeConfig();

void SetIntConfig(LPCSTR name, DWORD value);
int GetIntConfig(LPCSTR name);

void GetExeName(char (&buffer)[MAX_PATH]);