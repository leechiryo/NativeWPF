#pragma once

/*
 * ?��Model�L�꘢ModelChecker�^�V?�n�݈�N�C
 * ModelChecker�񋟗��꘢���@??Model�I?�I?����v�C�@��
 * ??��??����?���C?���@���ԉ�True�B
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
