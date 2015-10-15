#pragma once

#include "Model.h"
#include "ViewBase.h"

class ControllerBase {
private:
  ViewBase *m_v;

  void ContrllerMethod(ViewBase *v, Model *m) {
  }

  virtual ViewBase * InnerControl(Model *m) = 0;
};