#pragma once
#include "Element.h"
class Button :
  public Element
{
public:

  Button()
  {
  }

  virtual ~Button()
  {
  }

  virtual void DrawSelf() {
    // draw a border.
  }

  // onclick event handler.
};

