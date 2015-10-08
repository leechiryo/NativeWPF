#pragma once

#include "Element.h"

class ClickableElement : public Element {
private:
  ID2D1SolidColorBrush* m_pNormalBackgroundBrush;
  ID2D1SolidColorBrush* m_pHoverBackgroundBrush;
  ID2D1SolidColorBrush* m_pClickBackgroundBrush;

protected:
  virtual void CreateD2DResources() {
    HRESULT hr = m_pRenderTarget->CreateSolidColorBrush(
      D2D1::ColorF(0xcccccc),
      &m_pNormalBackgroundBrush);

    if (!SUCCEEDED(hr)) {
      throw new std::runtime_error("Failed to create the background.");
    }

    hr = m_pRenderTarget->CreateSolidColorBrush(
      D2D1::ColorF(0x999999),
      &m_pHoverBackgroundBrush);

    if (!SUCCEEDED(hr)) {
      SafeRelease(m_pNormalBackgroundBrush);
      throw new std::runtime_error("Failed to create the background.");
    }

    hr = m_pRenderTarget->CreateSolidColorBrush(
      D2D1::ColorF(0x666666),
      &m_pClickBackgroundBrush);

    if (!SUCCEEDED(hr)) {
      SafeRelease(m_pNormalBackgroundBrush);
      SafeRelease(m_pHoverBackgroundBrush);
      throw new std::runtime_error("Failed to create the background.");
    }
  }

  virtual void ReleaseD2DResource() {
    SafeRelease(m_pNormalBackgroundBrush);
    SafeRelease(m_pHoverBackgroundBrush);
    SafeRelease(m_pClickBackgroundBrush);
  }

public:
  ClickableElement(ID2D1HwndRenderTarget *rt) : Element(rt) {
    // Call virtual function of itself.
    CreateD2DResources();
  }

  virtual ~ClickableElement() {
    ReleaseD2DResource();
  }
};