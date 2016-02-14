#pragma once

#include "ModelBase.h"
#include "ViewBase.h"

class ControllerBase {
private:
  ViewBase *m_v;

  void ContrllerMethod(ViewBase *v, ModelBase *m) {
  }

  virtual ViewBase * InnerControl(ModelBase *m) = 0;
};