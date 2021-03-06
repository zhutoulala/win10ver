// win10ver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <map>
#include <string>
#include <windows.h>
#include <iostream>

typedef LONG(__stdcall *fnRtlGetVersion)(PRTL_OSVERSIONINFOW lpVersionInfo);

std::map<DWORD, std::string> WIN10VER = 
{
{ 10240,    "Windows 10 Threshold 1, Version: 1507, Release Date: July 29, 2015."},
{ 10586,    "Windows 10 Threshold 2, Version: 1511, Release Date: November 10, 2015."},
{ 14393,    "Windows 10 Redstone 1, Version: 1607, Release Date: August 2, 2016."},
{ 15063,    "Windows 10 Redstone 2, Version: 1703, Release Date: April 5, 2017."},
{ 16299,    "Windows 10 Redstone 3, Version: 1709, Release Date: October 17, 2017."},
{ 17134,    "Windows 10 Redstone 4, Version: 1803, Release Date: April 30, 2018."},
{ 17763,    "Windows 10 Redstone 5, Version: 1809, Release Date: October 30, 2018." }
};

int main()
{
    HMODULE hNtdll = GetModuleHandle(L"ntdll.dll");
    if (hNtdll == NULL)
    {
        std::cout << "Unable to get handle to ntdll.dll." << std::endl;
        return -1;
    }

    fnRtlGetVersion pRtlGetVersion = (fnRtlGetVersion)GetProcAddress(hNtdll, "RtlGetVersion");
    if (pRtlGetVersion == NULL)
    {
        std::cout << "Unable to get address of RtlGetVersion." << std::endl;
        return -1;
    }

    RTL_OSVERSIONINFOW VersionInfo = { 0 };
    VersionInfo.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
    NTSTATUS status = pRtlGetVersion(&VersionInfo);

    if (status != 0)
    {
        std::cout << "RtlGetVersion call failed." << std::endl;
        return -1;
    }

    switch (VersionInfo.dwMajorVersion)
    {
    case 5:
        if (VersionInfo.dwMinorVersion == 1)
        {
            std::cout << "Not running Windows 10, instead it's XP." << std::endl;
            return 0;
        }
    case 6:
        switch (VersionInfo.dwMinorVersion)
        {
        case 1:
            std::cout << "Not running Windows 10, instead it's Windows 7." << std::endl;
            return 0;
        case 2:
            std::cout << "Not running Windows 10, instead it's Windows 8." << std::endl;
            return 0;
        case 3:
            std::cout << "Not running Windows 10, instead it's Windows 8.1." << std::endl;
            return 0;
        }
    case 10:
        if (WIN10VER.find(VersionInfo.dwBuildNumber) != WIN10VER.end())
        {
            std::cout << WIN10VER[VersionInfo.dwBuildNumber] << std::endl;
            return 0;
        }
        if (VersionInfo.dwBuildNumber > WIN10VER.rbegin()->first)
        {
            std::cout << "Possible Windows 10 19H1, Version: 1903, still in preview, Release Plan: April, 2019." << std::endl;
            return 0;
        }
    }
    std::cout << "Unable to determine Windows version." << std::endl;
    return -1;
}

