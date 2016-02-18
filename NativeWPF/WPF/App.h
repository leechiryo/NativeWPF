#pragma once

/*
 * Every view class has a layout and model as its template parameters.
 * Every view instance has a layout object to manage its layout and
 * a model instance to bind.
 * 每个view类有两个模板参数，一个是Layout，一个是Model, 每个view的实例保存着一个
 * layout和model的引用。
 *
 * Controller will be an static class (which the controller method will
 * be a static method).
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

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <map>
#include <list>
#include <string>
#include "Controller.h"
#include "ModelBase.h"

using namespace std;

class ViewBase;

class App
{
private:
  map<string, ViewBase*> m_views;
  list<ModelBase *> m_models;

  static ID2D1Factory* s_pDirect2dFactory;
  static IDWriteFactory* s_pDWriteFactory;


public:
  static double DPI_SCALE_X;
  static double DPI_SCALE_Y;
  App();
  ~App();

  template <typename ViewType>
  ViewType* GetView(string);

  // Use this method to create view.
  // first parameter is the id of the view.
  // second parameter and its followers are the parameter
  // of the views constructor.
  template<typename V, typename... Args>
  V* CreateView(string id, Args... args) {
    if (m_views.find(id) == m_views.end()) {
      V* v = new V(args...);
      m_views.insert(make_pair(id, v));
    }
  }

  template<typename M, typename... Args>
  M* CreateModel(Args... args) {
    m_models.push_back(new M(args...));
  }
};