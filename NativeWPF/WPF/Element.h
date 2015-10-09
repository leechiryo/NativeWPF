#pragma once

#include <vector>
#include <map>
#include <d2d1.h>
#include <dwrite.h>
#include <windows.h>
#include <windowsx.h>
#include "CommonFunction.h"


class Element
{
public:
  typedef LRESULT(Element::*MessageFunction)(WPARAM wParam, LPARAM lParam);

private:
  std::map<UINT, std::vector<MessageFunction> > m_msgFuncTbl;

  static float DPI_SCALE_X;
  static float DPI_SCALE_Y;

  bool m_mouseIn;

protected:
  float m_left;
  float m_top;
  float m_right;
  float m_bottom;

  float m_borderLeft;
  float m_borderRight;
  float m_borderTop;
  float m_borderBottom;

  int m_zOrder;

  ID2D1HwndRenderTarget* m_pRenderTarget;

  std::vector<Element *> m_subElements;

  static ID2D1Factory* s_pDirect2dFactory;
  static IDWriteFactory* s_pDWriteFactory;

  Element(ID2D1HwndRenderTarget* pRenderTarget)
  {
    m_left = 0.0;
    m_top = 0.0;
    m_right = 0.0;
    m_bottom = 0.0;

    m_borderLeft = 0.0;
    m_borderRight = 0.0;
    m_borderTop = 0.0;
    m_borderBottom = 0.0;

    m_zOrder = 0;

    m_pRenderTarget = pRenderTarget;
  }

  // Derived class will implement this method to draw itself.
  virtual void DrawSelf() = 0;

  // Create the resources to draw itself.
  virtual void CreateD2DResources() = 0;

  // Destroy the resources.
  virtual void DestroyD2DResources() = 0;

  virtual void MouseEnter(float x, float y) {
  }

  virtual void MouseLeft(float x, float y) {
  }

  void GetPixelRect(RECT &rect){
    rect.left = DipsXToPixels(m_left);
    rect.top = DipsYToPixels(m_top);
    rect.right = DipsXToPixels(m_right);
    rect.bottom = DipsYToPixels(m_bottom);
  }

  void Draw() {
    DrawSelf();

    // TODO: Set coord (0, 0) to the top left corner
    // then draw all of the children
    for (auto e : m_subElements) {
      e->Draw();
    }
  }

  void CreateD2DEnvironment() {
    CreateD2DResources();

    for (auto e : m_subElements) {
      e->CreateD2DEnvironment();
    }
  }

  void DestroyD2DEnvironment() {
    for (auto e : m_subElements) {
      e->DestroyD2DEnvironment();
    }

    DestroyD2DResources();
  }

  char HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &result) {

    bool isMouseEvent = 0;
    int pixelX = 0;
    int pixelY = 0;
    if (msg == WM_MOUSEMOVE || msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP) {
      // When mouse event occured, we get the coordinate first.
      pixelX = GET_X_LPARAM(lParam);
      pixelY = GET_Y_LPARAM(lParam);
      isMouseEvent = true;
    }

    for (auto e : m_subElements) {
      if (isMouseEvent) {
        float dipX = PixelsToDipsX(pixelX);
        float dipY = PixelsToDipsY(pixelY);
        if (!e->HitTest(dipX, dipY)) {
          // Mouse event should be processed only when it is
          // occured in the element's area.
          if (e->m_mouseIn) {
            e->m_mouseIn = 0;
            e->MouseLeft(dipX, dipY);
          }
          continue;
        }
        else {
          if (!e->m_mouseIn) {
            e->m_mouseIn = 1;
            e->MouseEnter(dipX, dipY);
          }
        }
      }

      // child element will try to process the message first.
      char processed = e->HandleMessage(msg, wParam, lParam, result);

      // if child processed the message, then return the result
      // and the parent's event handler will be skipped.
      if (processed) return 1;
    }

    // if the child elements are not process the current
    // message, then try the parent element itself.
    if (m_msgFuncTbl.find(msg) != m_msgFuncTbl.end()) {
      for (auto f : m_msgFuncTbl[msg]){
        result = (this->*f)(wParam, lParam);
      }
      return 1;  // Notify to parent element that it processed the message.
    }

    // Notify to parent element that it doesn't processed the message.
    return 0;
  }

  template <typename T>
  float PixelsToDipsX(T x) {
    return static_cast<float>(x) / DPI_SCALE_X;
  }

  template <typename T>
  float PixelsToDipsY(T y) {
    return static_cast<float>(y) / DPI_SCALE_Y;
  }

  int DipsXToPixels(float x){
    return static_cast<int>(x * DPI_SCALE_X);
  }

  int DipsYToPixels(float y){
    return static_cast<int>(y * DPI_SCALE_Y);
  }

  virtual bool HitTest(float dipX, float dipY) {
    return dipX >= m_left && dipX <= m_right
      && dipY >= m_top && dipY <= m_bottom;
  }

  bool HitTest(int pixelsX, int pixelsY) {
    float dipX = PixelsToDipsX(pixelsX);
    float dipY = PixelsToDipsY(pixelsY);

    return HitTest(dipX, dipY);
  }

public:

  static HRESULT Initialize() {
    // create the d2d factory.
    HRESULT hr = D2D1CreateFactory(
      D2D1_FACTORY_TYPE_SINGLE_THREADED,
      &s_pDirect2dFactory);

    if (SUCCEEDED(hr)) {

      // create the dwrite factory.
      hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&s_pDWriteFactory));
    }

    float dpiX, dpiY;
    s_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);

    DPI_SCALE_X = dpiX / 96.0f;
    DPI_SCALE_Y = dpiY / 96.0f;

    return hr;
  }

  static void Uninitialize() {
    SafeRelease(s_pDWriteFactory);
    SafeRelease(s_pDirect2dFactory);
  }

  ~Element()
  {
    // NOTE: m_pRenderTarget is shared by all of the children in the 
    //       window, it will be created and released in the mainwindow class.
    for (auto e : m_subElements) {
      delete e;
      e = nullptr;
    }
  }

  template <typename T>
  T* CreateSubElement() {
    T *newT = new T(m_pRenderTarget);
    newT->CreateD2DResources();
    m_subElements.push_back(newT);
    return newT;
  }

  void SetPosition(float left, float top, float right, float bottom) {
    m_left = left;
    m_top = top;
    m_right = right;
    m_bottom = bottom;
  }

  void AddMessageFunc(UINT msg, MessageFunction func) {
    m_msgFuncTbl[msg].push_back(func);
  }

};