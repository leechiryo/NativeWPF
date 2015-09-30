#pragma once

#include <vector>

class Element
{
private:
  double m_left;
  double m_top;
  double m_right;
  double m_bottom;

  std::vector<Element *> m_subElements;

public:

  Element()
  {
    m_left = 0.0;
    m_top = 0.0;
    m_right = 0.0;
    m_bottom = 0.0;
  }

  virtual ~Element()
  {
    for (auto e : m_subElements) {
      delete e;
      e = nullptr;
    }
  }

  template <typename T>
  T* CreateSubElement() {
    T *newT = new T();
    m_subElements.push_back(newT);
    return newT;
  }

  virtual void Draw() {

  }
};

