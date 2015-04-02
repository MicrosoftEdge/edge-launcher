#include "stdafx.h"
#include "ProjectSpartanLauncher.h"
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
		hr = OpenUrlInProjectSpartan(argv[1]);
	}
	else
	{
		std::cout << "Please supply only one argument: <url>";
		hr = E_INVALIDARG;
	}

	CoUninitialize();

	return hr;
}


HRESULT OpenUrlInProjectSpartan(__in PCWSTR url)
{
	HRESULT hr = E_FAIL;
	CComPtr<IApplicationActivationManager> activationManager;
	LPCWSTR spartanAUMID = L"Microsoft.Windows.Spartan_cw5n1h2txyewy!Microsoft.Spartan.Spartan";
	hr = CoCreateInstance(CLSID_ApplicationActivationManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&activationManager));
	if (SUCCEEDED(hr))
	{
		DWORD newProcessId;
		hr = activationManager->ActivateApplication(spartanAUMID, url, AO_NONE, &newProcessId);
	}
	else
	{
		std::cout << L"Failed to launch Project Spartan";
	}
	return hr;
}
