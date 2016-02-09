#pragma once

#include <map>
#include <string>
#include "Controller.h"

using namespace std;

class App
{
private:
  map<string, ControllerBase*> m_controllers;
public:
  App();
  ~App();
};

