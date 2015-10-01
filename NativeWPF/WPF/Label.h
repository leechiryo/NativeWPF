#pragma once

#include "Element.h"

class Label : public Element {

private:
  static const int MAX_CHARS = 256;
  wchar_t m_text[MAX_CHARS+1];

public :
  Label() {
  }

  virtual ~Label() {
  }

  void SetText(const wchar_t *text) {
    wcscpy_s(m_text, MAX_CHARS+1, text);
  }

  virtual void DrawSelf() {

  }

};