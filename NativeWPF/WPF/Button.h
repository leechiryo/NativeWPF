#pragma once
#include "Element.h"
class Button :
  public Element
{
public:

  Button(ID2D1HwndRenderTarget *pRT) : Element(pRT) {
  }

  virtual ~Button() {
    // virtual destructor
  }

  virtual void DrawSelf() {
    // draw a border and have a label in it.
  }

  // onclick event handler.

  // onhover event handler. change the apperance of the button.
};