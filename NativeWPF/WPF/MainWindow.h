#pragma once

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include "../Resource.h"
#include "Element.h"
#include "CommonFunction.h"

class MainWindow : public Element
{
public:
  MainWindow() : Element(nullptr) {
    m_hwnd = nullptr;

    m_pLightSlateGrayBrush = nullptr;
    m_pCornflowerBlueBrush = nullptr;

    // Create window using windows api.
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
      // Call virtual function from the constructor.
      // This is what I want.
      CreateD2DResources();

      ShowWindow(m_hwnd, SW_SHOWNORMAL);
      UpdateWindow(m_hwnd);
    }
  }

  ~MainWindow() {

    // Call virtual function from the destructor.
    // This is what I want.
    DestoryD2DResources();

  }

  // Process and dispatch messages
  void RunMessageLoop() {
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  // Initialize device-dependent resources.
  virtual void CreateD2DResources() {
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
  }

  // Release device-dependent resource.
  virtual void DestoryD2DResources() {
    SafeRelease(m_pLightSlateGrayBrush);
    SafeRelease(m_pCornflowerBlueBrush);
    SafeRelease(m_pRenderTarget);
  }

  virtual void DrawSelf() {
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
  }

  void Update() {
    InvalidateRect(m_hwnd, NULL, false);
  }

private:


  // Draw content.
  HRESULT OnRender() {
    HRESULT hr = S_OK;

    if (SUCCEEDED(hr)) {
      m_pRenderTarget->BeginDraw();

      // Method from base class (Element).
      Draw();

      m_pRenderTarget->EndDraw();
    }

    if (hr == D2DERR_RECREATE_TARGET) {
      // Discard all of the device resources in the child window
      // and recreate them.
      hr = S_OK;
      DestoryD2DEnvironment();
      CreateD2DEnvironment();
    }

    return hr;
  }

  // Resize the render target.
  void OnResize(WORD width, WORD height) {
    m_right = width;
    m_bottom = height;
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
      MainWindow *pDemoApp = (MainWindow *)pcs->lpCreateParams;
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
      MainWindow *pDemoApp = reinterpret_cast<MainWindow *>(static_cast<LONG_PTR>(
        ::GetWindowLongPtr(hwnd, GWLP_USERDATA)));

      bool wasHandled = false;

      if (pDemoApp) {
        switch (message) {
        case WM_SIZE:
        {
          WORD width = LOWORD(lParam);
          WORD height = HIWORD(lParam);
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
  ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
  ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
};
