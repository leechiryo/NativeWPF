#pragma once
#include <Windows.h>

class Controller{
public:

  template<typename M>
  static bool Method1(M* m, WPARAM w, LPARAM l, LRESULT& r) {
    return false;
  }
};
