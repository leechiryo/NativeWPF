#pragma once

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include "../Resource.h"
#include "Element.h"
#include "CommonFunction.h"

class DemoApp : public Element
{
public:
  DemoApp() : Element(nullptr) {
    m_hwnd = nullptr;
    m_pRenderTarget = nullptr;
    m_pLightSlateGrayBrush = nullptr;
    m_pCornflowerBlueBrush = nullptr;
    m_pBlackBlueBrush = nullptr;
    m_pTextFormat = nullptr;
  }

  ~DemoApp() {
    SafeRelease(m_pCornflowerBlueBrush);
    SafeRelease(m_pLightSlateGrayBrush);
    SafeRelease(m_pBlackBlueBrush);
    SafeRelease(m_pRenderTarget);
    SafeRelease(m_pTextFormat);
  }

  // Register the window class and call methods for instantiating drawing resources
  HRESULT Initialize() {

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(LONG_PTR);
    wcex.hInstance = HINST_THISCOMPONENT;
    wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_NATIVEWPF));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"NativeWPFWindow";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassEx(&wcex);

    m_hwnd = CreateWindowEx(
      0,
      L"NativeWPFWindow",
      TEXT("Native WPF Demo"),
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      NULL,
      NULL,
      HINST_THISCOMPONENT,
      this); // pass the this pointer to window parameter

    if (m_hwnd) {
      ShowWindow(m_hwnd, SW_SHOWNORMAL);
      UpdateWindow(m_hwnd);
    }

    return S_OK;
  }

  // Process and dispatch messages
  void RunMessageLoop() {
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

private:

  // Initialize device-dependent resources.
  HRESULT CreateDeviceResources() {
    HRESULT hr = S_OK;

    if (!m_pRenderTarget) {
      RECT rc;
      GetClientRect(m_hwnd, &rc);

      D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

      hr = s_pDirect2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hwnd, size),
        &m_pRenderTarget);

      if (SUCCEEDED(hr)) {
        hr = m_pRenderTarget->CreateSolidColorBrush(
          D2D1::ColorF(D2D1::ColorF::LightSlateGray),
          &m_pLightSlateGrayBrush);
      }
      if (SUCCEEDED(hr)) {
        hr = m_pRenderTarget->CreateSolidColorBrush(
          D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
          &m_pCornflowerBlueBrush);
      }
      if (SUCCEEDED(hr)) {
        hr = m_pRenderTarget->CreateSolidColorBrush(
          D2D1::ColorF(D2D1::ColorF::Black),
          &m_pBlackBlueBrush);
      }

      s_pDWriteFactory->CreateTextFormat(
        L"Migu 1M",
        NULL,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        16.0,
        L"ja-JP",
        &m_pTextFormat);

      m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

      m_pTextFormat->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_UNIFORM, 50.0f, 40.0f);


      IDWriteRenderingParams *pRP;
      s_pDWriteFactory->CreateCustomRenderingParams(
        3.5f,  // gamma
        1.0f,  // enhancedContrast
        1.0f,  // clearTypeLevel
        DWRITE_PIXEL_GEOMETRY_RGB,
        DWRITE_RENDERING_MODE_CLEARTYPE_NATURAL_SYMMETRIC,
        &pRP);

      m_pRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
      m_pRenderTarget->SetTextRenderingParams(pRP);
    }

    return hr;
  }

  // Release device-dependent resource.
  void DiscardDeviceResources() {
    SafeRelease(m_pLightSlateGrayBrush);
    SafeRelease(m_pCornflowerBlueBrush);
    SafeRelease(m_pBlackBlueBrush);
    SafeRelease(m_pRenderTarget);
    SafeRelease(m_pTextFormat);
  }

  // Draw content.
  HRESULT OnRender() {
    HRESULT hr = S_OK;

    hr = CreateDeviceResources();

    if (SUCCEEDED(hr)) {
      m_pRenderTarget->BeginDraw();
      m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
      m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
      D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

      int width = static_cast<int>(rtSize.width);
      int height = static_cast<int>(rtSize.height);

      for (int x = 0; x < width; x += 10) {
        m_pRenderTarget->DrawLine(
          D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
          D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
          m_pLightSlateGrayBrush,
          0.5f);
      }

      for (int y = 0; y < height; y += 10) {
        m_pRenderTarget->DrawLine(
          D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
          D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
          m_pLightSlateGrayBrush,
          0.5f);
      }

      // render text.
      D2D1_RECT_F textRect = D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height);

      wchar_t *msg = L"Hello, Direct 2D world!\nこんにちは、Direct 2D プログラミングの世界！";
      m_pRenderTarget->DrawText(
        msg,
        lstrlenW(msg),
        m_pTextFormat,
        textRect,
        m_pBlackBlueBrush);

      m_pRenderTarget->EndDraw();
    }

    if (hr == D2DERR_RECREATE_TARGET) {
      hr = S_OK;
      DiscardDeviceResources();
    }

    return hr;
  }

  // Resize the render target.
  void OnResize(UINT width, UINT height) {
    if (m_pRenderTarget) {
      m_pRenderTarget->Resize(D2D1::SizeU(width, height));
    }
  }

  // The windows procedure.
  static LRESULT CALLBACK WndProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam) {

    LRESULT result = 0;

    if (message == WM_CREATE) {
      LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
      DemoApp *pDemoApp = (DemoApp *)pcs->lpCreateParams;
      ::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pDemoApp);

      // inform the application of the frame change.
      RECT rcClient;
      GetWindowRect(hwnd, &rcClient);
      SetWindowPos(
        hwnd,
        NULL,
        rcClient.left,
        rcClient.top,
        rcClient.right - rcClient.left,
        rcClient.bottom - rcClient.top,
        SWP_FRAMECHANGED);

      result = 1;
    }
    else {
      DemoApp *pDemoApp = reinterpret_cast<DemoApp *>(static_cast<LONG_PTR>(
        ::GetWindowLongPtr(hwnd, GWLP_USERDATA)));

      bool wasHandled = false;

      if (pDemoApp) {
        switch (message) {
        case WM_SIZE:
        {
          UINT width = LOWORD(lParam);
          UINT height = HIWORD(lParam);
          pDemoApp->OnResize(width, height);
          result = 0;
          wasHandled = true;
          break;
        }

        case WM_DISPLAYCHANGE:
          InvalidateRect(hwnd, NULL, false);
          result = 0;
          wasHandled = true;
          break;

        case WM_PAINT:
          pDemoApp->OnRender();
          ValidateRect(hwnd, NULL);
          result = 0;
          wasHandled = true;
          break;

          /*
          ** From MSDN:
          ** If the wParam parameter is FALSE, the application should return zero.
          ** When wParam is TRUE, simply returning 0 without processing the NCCALCSIZE_PARAMS rectangles
          ** will cause the client area to resize to the size of the window, including the window frame.
          ** This will remove the window frame and caption items from your window, leaving only the client
          ** area displayed.
          */
        case WM_NCCALCSIZE:
          result = 0;
          wasHandled = true;
          break;

        case WM_DESTROY:
          PostQuitMessage(0);
          result = 1;
          wasHandled = true;
          break;
        }
      }

      if (!wasHandled) {
        result = DefWindowProc(hwnd, message, wParam, lParam);
      }
    }

    return result;
  }

  HWND m_hwnd;
  ID2D1HwndRenderTarget* m_pRenderTarget;
  IDWriteTextFormat* m_pTextFormat;
  ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
  ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
  ID2D1SolidColorBrush* m_pBlackBlueBrush;
};
