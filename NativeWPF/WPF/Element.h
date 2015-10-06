#pragma once

#include <vector>
#include <d2d1.h>
#include <dwrite.h>
#include "CommonFunction.h"

class Element
{
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

  // Destory the resources.
  virtual void DestoryD2DResources() = 0;

  void Draw() {
    DrawSelf();

    // TODO: Set coord (0, 0) to the top left corner
    // then draw all of the children
    for (auto e : m_subElements) {
      e->Draw();
    }
  }

  void CreateD2DEnvironment(){
    CreateD2DResources();

    for (auto e : m_subElements){
      e->CreateD2DEnvironment();
    }
  }

  void DestoryD2DEnvironment(){
    for (auto e : m_subElements){
      e->DestoryD2DEnvironment();
    }

    DestoryD2DResources();
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
};