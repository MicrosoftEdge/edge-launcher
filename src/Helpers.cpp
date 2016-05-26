//
// Copyright (C) Microsoft. All rights reserved.
//

#include "stdafx.h"
#include "Helpers.h"
#include <atlsafe.h>
#include <DocObj.h>
#include <Mshtmhst.h>
#include <ExDisp.h>
#include <webapplication.h>
#include <sstream>

#define IDM_STARTDIAGNOSTICSMODE 3802 
#define CP_AUTO 50001 
#define VERSION_SIGNATURE 0xFEEF04BD

namespace Helpers
{
    BOOL EnumWindowsHelper(_In_ const function<BOOL(HWND)>& callbackFunc)
    {
        return ::EnumWindows([](HWND hwnd, LPARAM lparam) -> BOOL {
            return (*(function<BOOL(HWND)>*)lparam)(hwnd);
        }, (LPARAM)&callbackFunc);
    }

    BOOL EnumChildWindowsHelper(HWND hwndParent, _In_ const function<BOOL(HWND)>& callbackFunc)
    {
        return EnumChildWindows(hwndParent, [](HWND hwnd, LPARAM lparam) -> BOOL {
            return (*(function<BOOL(HWND)>*)lparam)(hwnd);
        }, (LPARAM)&callbackFunc);
    }

    bool IsWindowClass(_In_ const HWND hwnd, _In_ LPCWSTR pszWindowClass)
    {
        if (hwnd && pszWindowClass && *pszWindowClass)
        {
            const int BUFFER_SIZE = 100;
            WCHAR szClassname[BUFFER_SIZE];

            int result = ::GetClassName(hwnd, (LPWSTR)&szClassname, BUFFER_SIZE);
            if (result && _wcsicmp(szClassname, pszWindowClass) == 0)
            {
                return true;
            }
        }

        return false;
    }

    HRESULT GetDocumentFromSite(_In_ IUnknown* spSite, _Out_ CComPtr<IDispatch>& spDocumentOut)
    {
        ATLENSURE_RETURN_HR(spSite != nullptr, E_INVALIDARG);

        CComQIPtr<IHTMLDocument2> spDocument(spSite);
        if (spDocument.p != nullptr)
        {
            CComQIPtr<IDispatch> spDocDisp(spDocument);
            ATLENSURE_RETURN_HR(spDocDisp.p != nullptr, E_NOINTERFACE);

            spDocumentOut = spDocDisp;
            return S_OK;
        }
        else
        {
            CComQIPtr<IWebBrowser2> spBrowser2(spSite);
            if (spBrowser2 != nullptr)
            {
                CComPtr<IDispatch> spDocDisp;
                HRESULT hr = spBrowser2->get_Document(&spDocDisp);
                FAIL_IF_NOT_S_OK(hr);
                ATLENSURE_RETURN_HR(spDocDisp.p != nullptr, E_FAIL);

                spDocumentOut = spDocDisp;
                return hr;
            }
            else
            {
                CComQIPtr<IWebApplicationHost> webAppHost(spSite);
                if (webAppHost.p != nullptr)
                {
                    CComPtr<IHTMLDocument2> spDoc;
                    HRESULT hr = webAppHost->get_Document(&spDoc);
                    FAIL_IF_NOT_S_OK(hr);
                    CComQIPtr<IDispatch> spDocDisp(spDoc);
                    ATLENSURE_RETURN_HR(spDocDisp.p != nullptr, E_NOINTERFACE);

                    spDocumentOut = spDocDisp;
                    return S_OK;
                }
            }
        }

        return E_NOINTERFACE;
    }

    HRESULT GetDocumentFromHwnd(_In_ const HWND browserHwnd, _Out_ CComPtr<IHTMLDocument2>& spDocument)
    {
        DWORD_PTR messageResult = 0;
        LRESULT sendMessageResult = ::SendMessageTimeoutW(browserHwnd, Helpers::GetHtmlDocumentMessage(), 0L, 0L, SMTO_ABORTIFHUNG, 2000, &messageResult);

        if (sendMessageResult != 0 && messageResult != 0)
        {
            HMODULE* hModule = new HMODULE(::LoadLibraryEx(L"oleacc.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32));
            shared_ptr<HMODULE> spOleAcc(hModule, [](HMODULE* hModule) { ::FreeLibrary(*hModule); delete hModule; });
            if (spOleAcc)
            {
                auto pfObjectFromLresult = reinterpret_cast<LPFNOBJECTFROMLRESULT>(::GetProcAddress(*spOleAcc.get(), "ObjectFromLresult"));
                if (pfObjectFromLresult != nullptr)
                {
                    return pfObjectFromLresult(messageResult, IID_IHTMLDocument2, 0, reinterpret_cast<void**>(&spDocument.p));
                }
            }
        }

        return E_FAIL;
    }

    UINT GetHtmlDocumentMessage()
    {
        if (s_getHtmlDocumentMessage == 0)
        {
            s_getHtmlDocumentMessage = ::RegisterWindowMessageW(L"WM_HTML_GETOBJECT");
        }

        return s_getHtmlDocumentMessage;
    }

    CString FormatErrorMessage(_In_ DWORD ErrorCode)
    {
        TCHAR   *pMsgBuf = NULL;
        DWORD   nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPTSTR>(&pMsgBuf), 0, NULL);
        if (!nMsgLen)
        {
            return _T("FormatMessage fail");
        }
        CString sMsg(pMsgBuf, nMsgLen);
        LocalFree(pMsgBuf);
        return sMsg;
    }
}
