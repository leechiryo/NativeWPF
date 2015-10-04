#pragma once

#include <vector>
#include <d2d1.h>
#include <dwrite.h>
#include "CommonFunction.h"

class Element
{
protected:
  double m_left;
  double m_top;
  double m_right;
  double m_bottom;

  double m_borderLeft;
  double m_borderRight;
  double m_borderTop;
  double m_borderBottom;

  int m_zOrder;

  ID2D1HwndRenderTarget* m_pRenderTarget;

  std::vector<Element *> m_subElements;

protected:
  static ID2D1Factory* s_pDirect2dFactory;
  static IDWriteFactory* s_pDWriteFactory;

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

    return hr;
  }

  static void Uninitialize() {
    SafeRelease(s_pDWriteFactory);
    SafeRelease(s_pDirect2dFactory);
  }

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

  ~Element()
  {
    for (auto e : m_subElements) {
      delete e;
      e = nullptr;
    }
  }

  template <typename T>
  T* CreateSubElement() {
    T *newT = new T(m_pRenderTarget);
    m_subElements.push_back(newT);
    return newT;
  }

  // Nothing need to draw.
  // Derived class will implement this method to draw itself.
  virtual void DrawSelf() = 0;

  // Create the resources to draw itself.
  virtual void CreateResources(){}

  // Destory the resources.
  virtual void DestoryResources(){}

  void Draw() {
    DrawSelf();

    // TODO: Set coord (0, 0) to the top left corner
    // then draw all of the children
    for (auto e : m_subElements) {
      e->Draw();
    }
  }

  void SetPosition(double left, double top, double right, double bottom) {
    m_left = left;
    m_top = top;
    m_right = right;
    m_bottom = bottom;
  }
};