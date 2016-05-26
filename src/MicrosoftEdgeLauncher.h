#include "stdafx.h"
#include <Shobjidl.h>
#include <Mshtml.h>

struct EdgeTargetInfo 
{
    HWND hwnd;
    BSTR url;
    DWORD pid;
    CComPtr<IHTMLDocument2> spDoc;
};

HRESULT LaunchEdge(_In_ PCWSTR pszUrl, _In_ BOOL bKeepAlive);
HRESULT LaunchEdgeViaShellExec(_In_ PCWSTR pszUrl);
EdgeTargetInfo WatchForEdgeTab(_In_ PCWSTR pszUrl);
HRESULT EnumerateTargets(_Out_  std::vector<EdgeTargetInfo>& vTargets);
HRESULT WaitForProcessToExit(_In_ DWORD dwPid);
void ShowHelp();
HRESULT ShowLastError(_In_ PCWSTR pszErrorIntro);
int LifetimeTimeoutMs = INFINITE; 
