#include "stdafx.h"
#include "MicrosoftEdgeLauncher.h"
#include <Shobjidl.h>
#include <iostream>
#include <Psapi.h>
#include "Helpers.h"
#include <ctime>

int _tmain(int argc, _TCHAR* argv[])
{
    HRESULT hr = S_OK;
    hr = CoInitialize(NULL);

    PCWSTR pszUrl = L"";
    bool bKeepAlive = false;

    if (!SUCCEEDED(hr))
    {
        return hr;
    }
    if (argc == 2)
    {
        if ((wcscmp(argv[1], L"-?") == 0) | (wcscmp(argv[1], L"-h") == 0) | (wcscmp(argv[1], L"--help") == 0))
        {
            ShowHelp();
            hr = E_ABORT;
        }
        else
        {
            pszUrl = argv[1];
        }
    }
    else if (argc == 3) 
    {
        pszUrl = argv[1];
        if ((wcscmp(argv[2], L"-k") == 0) | (wcscmp(argv[2], L"--keepalive") == 0))
        {
            bKeepAlive = true;
        }
        else
        {
            std::cout << "Error: Expecting -k got: \n " << argv[2];
            std::cout << "\n";
            std::cout << "Use -h for usage info.";
            hr = E_ABORT;
        }
    }
    else
    {
        pszUrl = L"http://www.bing.com/";
    }

    hr = LaunchEdge(pszUrl, bKeepAlive);

    CoUninitialize();
    return hr;
}


HRESULT LaunchEdge(_In_ PCWSTR pszUrl, _In_ BOOL bKeepAlive)
{
    if(!bKeepAlive)
    {
        return LaunchEdgeViaShellExec(pszUrl);
    }

    HRESULT hr;
    DWORD dwProcessID;

    std::srand(std::time(0));

    int iRandomSeed = rand() % 1000 + 1;
    wstringstream ss;
    ss << L"http://example.com/?" << iRandomSeed;
    wstring wsInitialUrl = ss.str();
    PCWSTR pszInitialUrl = wsInitialUrl.c_str();

    hr = LaunchEdgeViaShellExec(pszInitialUrl);
    if (!SUCCEEDED(hr))
    {
        return hr;
    }
    
    EdgeTargetInfo info;
    info = WatchForEdgeTab(pszInitialUrl);
    if (info.pid == 0)
    {
        return E_NOINTERFACE;
    }

    if (info.pDoc == nullptr)
    {
        return E_NOINTERFACE;
    }

    CComPtr<IHTMLWindow2> spWindow;
    hr = info.pDoc->get_parentWindow(&spWindow);
    if (!SUCCEEDED(hr))
    {
        ShowLastError(L"Failed to get the IHTMLWindow2");
        return hr;
    }

    spWindow->navigate(SysAllocString(pszUrl));
    if (!SUCCEEDED(hr))
    {
        ShowLastError(L"Failed to navigate to launch URL");
        return hr;
    }

    hr = WaitForProcessToExit(info.pid);
    return hr;
}

HRESULT LaunchEdgeViaShellExec(PCWSTR pszUrl)
{
    HRESULT hr;
    SHELLEXECUTEINFOW sei = { sizeof sei };
    sei.lpVerb = L"open";
    std::wstring wsUrl(pszUrl);
    std::wstring concatted_stdstr = L"microsoft-edge:" + wsUrl;
    sei.lpFile = concatted_stdstr.c_str();
    hr = ShellExecuteExW(&sei);

    if (!SUCCEEDED(hr))
    {
        ShowLastError(L"Failed to launch Microsoft Edge via shell execute");
        return hr;
    }

    return hr;
}

void ShowHelp()
{
    std::cout << "Launches the Microsoft Edge browser.";
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "Usage:";
    std::cout << "\n";
    std::cout << "\tMicrosoftEdgeLauncher.exe [url] -k";
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\turl - The URL to open in Microsoft Edge.";
    std::cout << "\t-k  - Keep this program alive for as long as the launched process is alive.";
    std::cout << "\n";
    std::cout << "\n";
}

HRESULT ShowLastError(_In_ PCWSTR pszErrorIntro)
{
    DWORD dwError = GetLastError();

    CString strErorrMessage = Helpers::FormatErrorMessage(dwError);

    std::cout << "\nError ";
    std::cout << "\n";
    std::cout << "\n" << pszErrorIntro;
    std::cout << "\n";
    std::cout << "\nError code: " << dwError;
    std::cout << "\nError message:";
    std::cout << "\n" << strErorrMessage;
    std::cout << "\n";

    return HRESULT_FROM_WIN32(dwError);
}


HRESULT WaitForProcessToExit(_In_ DWORD dwPid)
{
    HRESULT hr = S_OK;
    DWORD dwResult; 
    HANDLE hndProcess;

    hndProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    if (hndProcess == NULL)
    {
        return ShowLastError(L"Error opening process");
    }

    std::cout << "\nWaiting for process " << dwPid <<" to exit...";
    dwResult = WaitForSingleObject(hndProcess, LifetimeTimeoutMs);

    switch (dwResult)
    {
    case WAIT_OBJECT_0:
        return S_OK;
        break;
    case WAIT_TIMEOUT:
        std::cout << "\nTimed out waiting for process to exit.";
        return E_FAIL;
        break;
    default:
        return ShowLastError(L"Error wating for process exit.");
        break;
    }

    return hr;
}

EdgeTargetInfo WatchForEdgeTab(_In_ PCWSTR pszUrl)
{
    EdgeTargetInfo info = { 0 };
    int loopCounter = 0;
    int waitTimeMS = 1000;
    int maxTries = 20;

    do
    {
        Sleep(waitTimeMS);
        loopCounter++;
        vector<EdgeTargetInfo> vTargets;

        EnumerateTargets(vTargets);

        for (size_t i = 0; i < vTargets.size(); i++)
        {
            EdgeTargetInfo info = vTargets[i];
            if ((wcscmp(info.url, pszUrl) == 0)) {
                return info;
            }
        }

        if (loopCounter > maxTries)
        {
            std::cout << "\nCouldn't find Edge URL with URL: " << pszUrl;
            std::cout << "\nFound";

            for (size_t i = 0; i < vTargets.size(); i++)
            {
                EdgeTargetInfo info = vTargets[i];
                std::cout << "\n" << info.url;
            }
            
            return{ 0 };
        }
    } while (info.pid == 0);

    return info;
}

HRESULT EnumerateTargets(vector<EdgeTargetInfo>& vTargets)
{
    vTargets.empty();
    Helpers::EnumWindowsHelper([&](HWND hwndTop) -> BOOL
    {
        Helpers::EnumChildWindowsHelper(hwndTop, [&](HWND hwnd) -> BOOL
        {
            if (Helpers::IsWindowClass(hwnd, L"Internet Explorer_Server"))
            {
                bool isEdgeContentProcess = false;

                DWORD processId;
                ::GetWindowThreadProcessId(hwnd, &processId);

                CString processName;
                CHandle handle(::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId));
                if (handle)
                {
                    DWORD length = ::GetModuleFileNameEx(handle, nullptr, processName.GetBufferSetLength(MAX_PATH), MAX_PATH);
                    processName.ReleaseBuffer(length);
                    isEdgeContentProcess = (processName.Find(L"MicrosoftEdgeCP.exe") == processName.GetLength() - 19);

                    processName = ::PathFindFileNameW(processName);
                }

                if (isEdgeContentProcess)
                {
                    CComPtr<IHTMLDocument2> spDocument;
                    HRESULT hr = Helpers::GetDocumentFromHwnd(hwnd, spDocument);
                    if (hr == S_OK)
                    {
                        EdgeTargetInfo i;
                        i.hwnd = hwnd;
                        i.pid = processId;
                        i.pDoc = spDocument; 

                        hr = spDocument->get_URL(&i.url);
                        if (hr != S_OK)
                        {
                            i.url = L"unknown";
                        }
                        vTargets.push_back(i);
                    }
                }
            }

            return TRUE;
        });

        return TRUE;
    });

    return S_OK;
}
