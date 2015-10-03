#pragma once

#include <d2d1.h>
#include <dwrite.h>
#include "Element.h"

class Label : public Element {

private:
  static const int MAX_CHARS = 256;
  wchar_t m_text[MAX_CHARS + 1];
  wchar_t m_font[MAX_CHARS + 1];
  wchar_t m_locale[MAX_CHARS + 1];

  DWRITE_FONT_WEIGHT m_fontWeight;
  DWRITE_FONT_STYLE m_fontStyle;
  DWRITE_FONT_STRETCH m_fontStrech;

  DWRITE_TEXT_ALIGNMENT m_alignment;

  D2D1::ColorF::Enum m_color;

  ID2D1SolidColorBrush* m_pBlackBlueBrush;
  IDWriteTextFormat* m_pTextFormat;

public:
  Label() {
  }

  virtual ~Label() {
  }

  void SetText(const wchar_t *text) {
    wcscpy_s(m_text, MAX_CHARS + 1, text);
  }

  virtual void DrawSelf() {

  }

};