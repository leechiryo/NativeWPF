#pragma once

#include "Element.h"

class Label : public Element {

private:
  wchar_t m_text[128];

public :
  Label() {
  }

  void SetText(const wchar_t *text) {
    wcscpy_s(m_text, 128, text);
  }

  virtual ~Label() {

  }
};