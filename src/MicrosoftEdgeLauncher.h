#include "stdafx.h"
#include <Shobjidl.h>
#include <vector>

struct EdgeTargetInfo {
    HWND hwnd;
    PCWSTR title;
    PCWSTR url;
    PCWSTR processName;
    DWORD pid;
};

HRESULT LaunchEdge(_In_ PCWSTR pszUrl, _Out_ DWORD pid);
EdgeTargetInfo WatchForEdgeTab(_In_ PCWSTR pszUrl);
HRESULT EnumerateTargets(_Out_  std::vector<EdgeTargetInfo>& vTargets);
HRESULT AttachLifeTime(_In_ DWORD dwPid);
void ShowHelp();
HRESULT ShowLastError(_In_ PCWSTR pszErrorIntro);
int LifetimeTimeoutMs = INFINITE; 
