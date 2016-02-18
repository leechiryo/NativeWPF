#pragma once
#include <list>
#include <windows.h>
#include <windowsx.h>
#include "ModelBase.h"
#include "App.h"

using namespace std;

class ViewBase {
private:
  list<ViewBase*> m_subViews;
  ViewBase* m_parentView;
  bool m_hide;

  double m_left;
  double m_top;
  double m_right;
  double m_bottom;
  int m_width;
  int m_height;

  bool m_mouseIn;

protected:
  virtual bool HandleEvent(UINT msg, WPARAM w, LPARAM l, LRESULT&r) = 0;

  ViewBase() {
  }

public:
  int GetWidth() {
    return m_width;
  }

  int GetHeight() {
    return m_height;
  }

  void AddChild(ViewBase* v) {
    m_subViews.push_back(v);
  }

  // Derived class will implement this method to draw itself.
  virtual void DrawSelf() = 0;

  // Create the resources to draw itself.
  virtual void CreateD2DResources() = 0;

  // Destroy the resources.
  virtual void DestroyD2DResources() = 0;

  // Will be called when the mouse entered from the outer of the view.
  virtual void MouseEnter(double x, double y) {
  }

  // Will be called when the mouse left from the inner of the view.
  virtual void MouseLeft(double x, double y) {
  }

  void Draw() {
    if (!m_hide) {
      DrawSelf();
      for (auto v : m_subViews) {
        v->Draw();
      }
    }
  }

  void CreateD2DEnvironment() {
    CreateD2DResources();
    for (auto e : m_subViews) {
      e->CreateD2DEnvironment();
    }
  }

  void DestroyD2DEnvironment() {
    for (auto e : m_subViews) {
      e->DestroyD2DEnvironment();
    }
    DestroyD2DResources();
  }

  void Hide() {
    m_hide = true;
    m_parentView->Draw();
  }

  bool HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &result) {

    bool isMouseEvent = 0;
    int pixelX = 0;
    int pixelY = 0;
    if (msg == WM_MOUSEMOVE || msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP) {
      // When mouse event occured, we get the coordinate first.
      pixelX = GET_X_LPARAM(lParam);
      pixelY = GET_Y_LPARAM(lParam);
      isMouseEvent = true;
    }

    for (auto e : m_subViews) {
      if (isMouseEvent) {
        double dipX = PixelsToDipsX(pixelX);
        double dipY = PixelsToDipsY(pixelY);
        if (!e->HitTest(dipX, dipY)) {
          // Mouse event should be processed only when it is
          // occured in the element's area.
          if (e->m_mouseIn) {
            e->m_mouseIn = false;
            e->MouseLeft(dipX, dipY);
          }
          continue;
        }
        else {
          if (!e->m_mouseIn) {
            e->m_mouseIn = true;
            e->MouseEnter(dipX, dipY);
          }
        }
      }

      // child element will try to process the message first.
      bool processed = e->HandleMessage(msg, wParam, lParam, result);

      // if child processed the message, then return the result
      // and the parent's event handler will be skipped.
      if (processed) return 1;
    }

    // if the child elements are not process the current
    // message, then try the parent element itself.
    return HandleEvent(msg, wParam, lParam, result);
  }

  template <typename T>
  double PixelsToDipsX(T x) {
    return static_cast<double>(x) / App::DPI_SCALE_X;
  }

  template <typename T>
  double PixelsToDipsY(T y) {
    return static_cast<double>(y) / App::DPI_SCALE_Y;
  }

  int DipsXToPixels(double x){
    return static_cast<int>(x * (App::DPI_SCALE_X));
  }

  int DipsYToPixels(double y){
    return static_cast<int>(y * App::DPI_SCALE_Y);
  }

  virtual bool HitTest(double dipX, double dipY) {
    return dipX >= m_left && dipX <= m_right
      && dipY >= m_top && dipY <= m_bottom;
  }

  bool HitTest(int pixelsX, int pixelsY) {
    double dipX = PixelsToDipsX(pixelsX);
    double dipY = PixelsToDipsY(pixelsY);

    return HitTest(dipX, dipY);
  }
};