#pragma once

#include "ClickableElement.h"
#include "Label.h"

class Button :
  public ClickableElement
{
  friend class Element;
  
private:
  Label * m_pText;

protected:

  Button(ID2D1HwndRenderTarget *pRT) : ClickableElement(pRT){
    m_pText = CreateSubElement<Label>();
    m_pText->SetPosition(m_left, m_top, m_right, m_bottom);
    m_pText->SetText(L"Click!");
    m_pText->SetColor(0x333333);
    m_pText->SetFont(L"Source Code Pro",
      DWRITE_FONT_WEIGHT_REGULAR,
      DWRITE_FONT_STYLE_NORMAL,
      DWRITE_FONT_STRETCH_NORMAL, 14.0f);
    m_pText->SetAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    m_pText->SetVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    AddMessageFunc(WM_LBUTTONDOWN, (MessageFunction)(&Button::HandleMessage_LBUTTONDOWN));
    AddMessageFunc(WM_LBUTTONUP, (MessageFunction)(&Button::HandleMessage_LBUTTONUP));
  }

  virtual ~Button() {
    // virtual destructor
  }

  // onhover event handler. change the apperance of the button.
  virtual void MouseEnter(float x, float y) {
    m_pText->SetColor(0x1149cc);
    ClickableElement::MouseEnter(x, y);
  }

  virtual void MouseLeft(float x, float y) {
    m_pText->SetColor(0x333333);
    ClickableElement::MouseLeft(x, y);
  }

  // onclick event handler.
  LRESULT HandleMessage_LBUTTONDOWN(WPARAM wParam, LPARAM lParam){
    m_pText->SetColor(0xcccccc);
    return 0;
  }

  LRESULT HandleMessage_LBUTTONUP(WPARAM wParam, LPARAM lParam){
    m_pText->SetColor(0x1149cc);
    return 0;
  }

  virtual void DrawSelf(){
    m_pText->SetPosition(m_left, m_top, m_right, m_bottom);
    ClickableElement::DrawSelf();
  }

};