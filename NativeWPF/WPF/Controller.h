#pragma once
#include <Windows.h>

class Controller{
public:

  template<typename M>
  bool Method1(M* m, WPARAM w, LPARAM l, LRESULT& r) {
    return false;
  }
};
