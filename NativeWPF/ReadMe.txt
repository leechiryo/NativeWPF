Desktop MVC in C++

M - Model
V - View
C - Controller

                       +----------------+
        +------------- |   Controller   | <--+-------------+
        |              +----------------+    |             |
        |                                    |             |
        v                                    |             |
+----------------+                           |             |
|     Model      |                           |             |
+----------------+                           |             |
        ^                                    v             |
        |                     +----------------+           |
        +------------------>  |      View      |           |
        |                     +----------------+           |
        |                                                  |
        |                     +----------------+           |
        +------------------>  |  Sub View      |  <--------+
        |                     +----------------+           |
        |                                                  |
        |                     +----------------+           |
        +------------------>  |  Sub View      |  <--------+
                              +----------------+





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