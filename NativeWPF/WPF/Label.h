#pragma once

#include <d2d1.h>
#include <dwrite.h>
#include "Element.h"
#include "CommonFunction.h"

class Label : public Element {

private:
  static const int MAX_CHARS = 256;
  wchar_t m_text[MAX_CHARS + 1];
  wchar_t m_font[MAX_CHARS + 1];
  wchar_t m_locale[MAX_CHARS + 1];

  DWRITE_FONT_WEIGHT m_fontWeight;
  DWRITE_FONT_STYLE m_fontStyle;
  DWRITE_FONT_STRETCH m_fontStretch;

  DWRITE_TEXT_ALIGNMENT m_alignment;

  UINT32 m_color;

  ID2D1SolidColorBrush* m_pBrush;
  IDWriteTextFormat* m_pTextFormat;

  void UpdateTextFormat() {
    SafeRelease(m_pTextFormat);

    HRESULT hr = s_pDWriteFactory->CreateTextFormat(
      m_font,
      NULL,
      m_fontWeight,
      m_fontStyle,
      m_fontStretch,
      16.0,
      L"ja-JP",
      &m_pTextFormat);

    if (!SUCCEEDED(hr)) {
      throw new std::runtime_error("Failed to update the text format.");
    }
  }

public:
  Label(ID2D1HwndRenderTarget *rt) : Element(rt) {
    m_color = 0x333333;
    m_fontWeight = DWRITE_FONT_WEIGHT_REGULAR;
    m_fontStyle = DWRITE_FONT_STYLE_NORMAL;
    m_fontStretch = DWRITE_FONT_STRETCH_NORMAL;

    wcscpy_s(m_font, MAX_CHARS + 1, L"Migu 1M");

    HRESULT hr = s_pDWriteFactory->CreateTextFormat(
      m_font,
      NULL,
      m_fontWeight,
      m_fontStyle,
      m_fontStretch,
      16.0,
      L"ja-JP",
      &m_pTextFormat);

    if (!SUCCEEDED(hr)) {
      throw new std::runtime_error("Failed to create the text format.");
    }

    hr = m_pRenderTarget->CreateSolidColorBrush(
      D2D1::ColorF(m_color),
      &m_pBrush);

    if (!SUCCEEDED(hr)) {
      SafeRelease(m_pTextFormat);
      throw new std::runtime_error("Failed to create the brush.");
    }
  }

  virtual ~Label() {
    SafeRelease(m_pBrush);
    SafeRelease(m_pTextFormat);
  }

  void SetText(const wchar_t *text) {
    wcscpy_s(m_text, MAX_CHARS + 1, text);
  }

  void SetFont(DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch) {
    m_fontWeight = weight;
    m_fontStyle = style;
    m_fontStretch = stretch;
    UpdateTextFormat();
  }

  void SetColor(UINT32 rgbColor) {
    m_color = rgbColor;
    SafeRelease(m_pBrush);

    HRESULT hr = m_pRenderTarget->CreateSolidColorBrush(
      D2D1::ColorF(m_color),
      &m_pBrush);

    if (!SUCCEEDED(hr)) {
      throw new std::runtime_error("Failed to create the brush.");
    }
  }

  void SetFont(const wchar_t * font) {
    wcscpy_s(m_font, MAX_CHARS + 1, font);
  }

  virtual void DrawSelf() {
    D2D1_RECT_F textRect = D2D1::RectF(m_left, m_top, m_right, m_bottom);

    m_pRenderTarget->DrawText(
      m_text,
      lstrlenW(m_text),
      m_pTextFormat,
      textRect,
      m_pBrush);
  }

};