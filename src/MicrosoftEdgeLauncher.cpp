#include "stdafx.h"
#include "MicrosoftEdgeLauncher.h"
#include <Shobjidl.h>
#include <iostream>

int _tmain(int argc, _TCHAR* argv[]){
    HRESULT hr = E_FAIL;
    hr = CoInitialize(NULL);
    if (!SUCCEEDED(hr)){
        return hr;
    }
    if (2 == argc){   
		if ((argv[1] == L"-h") | (argv[1] == L"-help")) {
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
			hr = E_ABORT;
		}
		else {
			hr = OpenUrlInMicrosoftEdge(argv[1]);
		}
    }
    else{
		hr = OpenUrlInMicrosoftEdge(L"");
    }
    CoUninitialize();
    return hr;
}

HRESULT OpenUrlInMicrosoftEdge(__in PCWSTR url){
	HRESULT hr = E_FAIL;
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	SHELLEXECUTEINFOW sei = { sizeof sei };
	sei.lpVerb = L"open";
	std::wstring mywstring(url);
	std::wstring concatted_stdstr = L"microsoft-edge:" + mywstring;
	sei.lpFile = concatted_stdstr.c_str();
	hr = ShellExecuteExW(&sei);
	if (FAILED(hr)) {
		std::cout << L"Failed to launch Microsoft Edge";
	}
	return hr;
}
