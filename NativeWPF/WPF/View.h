#pragma once

/*
 * Every view class has a layout and model as its template parameters.
 * Every view instance has a layout object to manage its layout and
 * a model instance to bind.
 *
 * Controller will be an static class (of which method will be static).
 *
 * When an event happened on a view, the view will call a controller method
 * to handle the event, pass the model it bound as the calling parameter.
 * The controller will then get the model and do some process and finally
 * update some model in the app (may be the model that other view bound).
 *
 * The model of the view that event happened will got by the controller
 * through the calling parameter. Contoller can get the other view's model
 * by get the other view first by their id, then get the model from the 
 * corresponding view.
 *
 * So, the app must has some mechanism to store all the view's instance and its Id.
 * When the controller asks, the app should return the view's instance by the Id.
 */

#include <map>
#include <Windows.h>
#include "ViewBase.h"

using namespace std;

/*
 * Many views respect an element in the window,
 * which has a GridLayout or CanvasLayout.
*/
template <typename Layout, typename Model>
class View : public ViewBase{

  typedef bool(*EventHandler) (Model*, WPARAM, LPARAM, LRESULT&);

public:
  View(Model* m, Layout* l) : ViewBase() {
    m_pModel = m;
    _l = l;
    _l->SetView(this);
  }

  template <typename... LayoutArgs>
  void AddSubView(ViewBase* v, LayoutArgs... args) {
    AddChild(v);
    _l->AddSubView(v, args...);
  }

  void AddEvent(UINT msg, EventHandler h) {
    if (m_events.find(msg) != m_events.end()) {
      m_events[msg].push_back(h);
    }
    else {
      m_events.insert(make_pair(msg, list<EventHandler>(h)));
    }
  }

  bool HandleEvent(UINT msg, WPARAM w, LPARAM l, LRESULT&r) {
    if (m_events.find(msg) != m_events.end()) {
      bool processed = true;
      for (auto eh : m_events[msg]) {
        processed = processed && m_events[msg](m_pModel, w, l, r);
      }
      // 每个处理函数都返回True（即，所有事件的处理都不再向父View传递）时，将返回True。
      // 如果有一个处理函数返回False，则将返回False。如此会导致父View的事件处理函数被调用。
      return processed;
    }
    return false;
  }

  Model* GetModel(){
    return m_pModel;
  }

private:
  Layout* _l;
  map<UINT, list<EventHandler>> m_events;
  Model* m_pModel;
};