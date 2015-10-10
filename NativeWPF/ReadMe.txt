Desktop MVC in C++

M - Model
V - View
C - Controller                  +------------------------------------------------------------+
                                |                 (3') Create a new model and new view       |
 (3) Manipulate Model  +----------------+                                                    |
        +------------- |   Controller   | <--+---------------------------------+             |   
        |              +----------------+    |                                 |             v
        |                                    |                                 |  +-------------------------+        +----------------+
        v                                    |                                 |  |  New View               |<------>| New Model      |
+----------------+                           |                                 |  |                         |        +----------------+
|     Model      |                           |                                 |  |   (4') Event Occured    |
+----------------+                           |                                 |  |  +--------------------+ |  (5') Invoke another controller
        ^                                    v                                 |  |  |  Sub View          | |-------------------------...
        |                     +-------------------------+                      |  |  |                    | |       use a string as reference
        +------------------>  |  View                   |                      |  |  | +----------------+ | |       to find a controller
 (4) Notify the model changed |                         |                      |  |  | | Sub Sub View   | | |
                              |  (1) Event Occured      |                      |  |  | +----------------+ | |
                              |  +--------------------+ |                      |  |  +--------------------+ |
                              |  |  Sub View          | |                      |  +-------------------------+ 
                              |  |                    | | (2) Invoke Controller|   
                              |  | +----------------+ | |  --------------------+   
                              |  | | Sub Sub View   | | |     use a string as reference
                              |  | +----------------+ | |     to find a controller
                              |  +--------------------+ |
                              +-------------------------+
                                 (5) Update the view

class App{
private:
  map<string, Controller*> m_controllers;
  map<string, View*> m_views;
  map<string, Model*> m_models;
  map<string, Model*> m_oldModels; // compare with m_models will make us know the model is changed.
public:
  Controller * GetController(string);
  View * GetView(string);
  Model * GetModel(string);

  template <class MT>
  MT * CreateNewModel(const string & s, MT * m){
    MT * m = new MT();
    m_model[s] = m;
    return m;
  }

  template <class VT, class MT>
  VT * CreateNewView(const string & s, MT * m){
    VT * v = new VT();
    m_views[s] = v;
    v->SetModel(m);
    return v;
  }
}


class Model{
private:
  vector<View*> m_views;
  void AddView(View * v){
    m_views.push_back(v);
  }

public:
  int m_member1;
  float m_member2;
  void NotifyChanged(){
    for(auto v : m_views){
      v->Update();
    }
  }
};

class View{
private:
  Model *m_model;
  Controller *m_controller;
  vector<View *> m_subViews;

public:
  Model * GetModel(){
    return m_model;
  }

  void SetModel(Model * m){
    m->AddView(this);
    m_model = m;
  }

  void Update(){
    // use the model value to update itself.
    // check if the model member which is showed
    // in the view is changed.

    // if changed, then call invalidate to update 
    // the screen.

    // Is there a common way to check if the model 
    // is changed?
    Invalidate();
  }

  void AddSubView(View *v){
    v->SetModel(m_model);
    m_subViews.push_back(v);
  }
};

class Controller{
private:
    View * m_v1;
    View * m_v2;

protected:
    View * m_mainView;

public :
  Controller(){
    m_v1 = new View();
    m_v2 = new View();
    m_mainView = m_v1;
  }

  ~Controller(){
    delete m_v1;
    delete m_v2;
  }

  View * ControlFunction(Model *m){
    m->m_member1 = newVal1;
    m->m_member2 = newVal2;
    m->NotifyChanged();
  }

  Startup(){
    m_mainView->Show();
  }
};