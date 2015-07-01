#include "stdafx.h"
#include "MicrosoftEdgeLauncher.h"
#include <Shobjidl.h>
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
    HRESULT hr = E_FAIL;
    hr = CoInitialize(NULL);
    if (!SUCCEEDED(hr))
    {
        return hr;
    }

    if (2 == argc)
    {
        // First param is the path to the exe         
            hr = OpenUrlInMicrosoftEdge(argv[1]);
    }
    else
    {
        std::cout << "Launches the Microsoft Edge browser.";
        std::cout << "\n";
        std::cout << "\n";
        std::cout << "Usage:";
        std::cout << "\n";
        std::cout << "\tMicrosoftEdgeLauncher.exe [url]";
        std::cout << "\n";
        std::cout << "\n";
        std::cout << "\turl - The URL to open in Microsoft Edge.";
        std::cout << "\n";
        std::cout << "\n";
        hr = E_INVALIDARG;
    }

    CoUninitialize();

    return hr;
}


HRESULT OpenUrlInMicrosoftEdge(__in PCWSTR url)
{
    HRESULT hr = E_FAIL;
    CComPtr<IApplicationActivationManager> activationManager;
    LPCWSTR edgeAUMID = L"Microsoft.MicrosoftEdge_8wekyb3d8bbwe!MicrosoftEdge";
    hr = CoCreateInstance(CLSID_ApplicationActivationManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&activationManager));
    if (SUCCEEDED(hr))
    {
        DWORD newProcessId;
        hr = activationManager->ActivateApplication(edgeAUMID, url, AO_NONE, &newProcessId);
    }
    else
    {
        std::cout << L"Failed to launch Microsoft Edge";
    }
    return hr;
}
