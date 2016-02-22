#pragma once

/*
 * ?个Model有一个ModelChecker与之?系在一起，
 * ModelChecker提供了一个方法??Model的?的?化情况，如果
 * ??了??生了?化，?方法将返回True。
*/

class ModelCheckerBase {
public:
  virtual bool ModelChanged() = 0;
  virtual void * GetModelAddr() = 0;
};

template <typename T>
class ModelChecker : ModelCheckerBase{
private:
  T modelCopy;
  const T* pModel;

public:
  ModelChecker(const T &model) : modelCopy(model) {
    pModel = &model;
  }

  bool ModelChanged() {
    bool changed = (modelCopy != *pModel);
    if (changed) {
      modelCopy = *pModel;
    }
    return changed;
  }

  const void * GetModelAddr() {
    return static_cast<void*>(pModel);
  }
};
