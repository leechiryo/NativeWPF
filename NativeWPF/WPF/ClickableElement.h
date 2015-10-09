#pragma once

#include "Element.h"

class ClickableElement : public Element {
  friend class Element;
private:
  ID2D1SolidColorBrush* m_pNormalBackgroundBrush;
  ID2D1SolidColorBrush* m_pHoverBackgroundBrush;
  ID2D1SolidColorBrush* m_pClickBackgroundBrush;
  ID2D1SolidColorBrush* m_pBackgroundBrush;

  LRESULT HandleMessage_LBUTTONDOWN(WPARAM wParam, LPARAM lParam){
    m_pBackgroundBrush = m_pClickBackgroundBrush;

    RECT rect;
    GetPixelRect(rect);

    InvalidateRect(m_pRenderTarget->GetHwnd(), &rect, 0);
    return 0;
  }

  LRESULT HandleMessage_LBUTTONUP(WPARAM wParam, LPARAM lParam){
    m_pBackgroundBrush = m_pHoverBackgroundBrush;

    RECT rect;
    GetPixelRect(rect);

    InvalidateRect(m_pRenderTarget->GetHwnd(), &rect, 0);
    return 0;
  }

protected:
  virtual void CreateD2DResources() {
    HRESULT hr = m_pRenderTarget->CreateSolidColorBrush(
      D2D1::ColorF(0xcccccc),
      &m_pNormalBackgroundBrush);

    if (!SUCCEEDED(hr)) {
      throw new std::runtime_error("Failed to create the background brush.");
    }

    hr = m_pRenderTarget->CreateSolidColorBrush(
      D2D1::ColorF(0x999999),
      &m_pHoverBackgroundBrush);

    if (!SUCCEEDED(hr)) {
      SafeRelease(m_pNormalBackgroundBrush);
      throw new std::runtime_error("Failed to create the background brush.");
    }

    hr = m_pRenderTarget->CreateSolidColorBrush(
      D2D1::ColorF(0x666666),
      &m_pClickBackgroundBrush);

    if (!SUCCEEDED(hr)) {
      SafeRelease(m_pNormalBackgroundBrush);
      SafeRelease(m_pHoverBackgroundBrush);
      throw new std::runtime_error("Failed to create the background brush.");
    }
  }

  virtual void DestroyD2DResources() {
    SafeRelease(m_pNormalBackgroundBrush);
    SafeRelease(m_pHoverBackgroundBrush);
    SafeRelease(m_pClickBackgroundBrush);
  }

  virtual void DrawSelf(){
    D2D1_RECT_F rect;
    rect.left = m_left;
    rect.right = m_right;
    rect.top = m_top;
    rect.bottom = m_bottom;
    m_pRenderTarget->FillRectangle(rect, m_pBackgroundBrush);
  }

  virtual void MouseEnter(float x, float y) {
    m_pBackgroundBrush = m_pHoverBackgroundBrush;

    RECT rect;
    GetPixelRect(rect);

    InvalidateRect(m_pRenderTarget->GetHwnd(), &rect, 0);
  }

  virtual void MouseLeft(float x, float y) {
    m_pBackgroundBrush = m_pNormalBackgroundBrush;

    RECT rect;
    GetPixelRect(rect);

    InvalidateRect(m_pRenderTarget->GetHwnd(), &rect, 0);
  }

public:
  ClickableElement(ID2D1HwndRenderTarget *rt) : Element(rt) {
    // Call virtual function of itself.
    CreateD2DResources();
    m_pBackgroundBrush = m_pNormalBackgroundBrush;

    AddMessageFunc(WM_LBUTTONDOWN, (MessageFunction)(&ClickableElement::HandleMessage_LBUTTONDOWN));
    AddMessageFunc(WM_LBUTTONUP, (MessageFunction)(&ClickableElement::HandleMessage_LBUTTONUP));
  }

  virtual ~ClickableElement() {
    // Call virtual function of itself.
    DestroyD2DResources();
  }
};