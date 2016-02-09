#pragma once

#include "Model.h"
#include "ControllerBase.h"

template <class V, class M>
class Controller: public ControllerBase{
public:
  virtual ViewBase * InnerControl(Model *m) {
    M *realM = static_cast<M*> m;
    return ControllerFunc(realM);
  }
  virtual V * ControllerFunc(M *m) = 0;
};
