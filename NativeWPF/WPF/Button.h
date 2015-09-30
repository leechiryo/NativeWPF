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

  virtual void Draw() {
    // draw a border.
  }

  // onclick event handler.
};

