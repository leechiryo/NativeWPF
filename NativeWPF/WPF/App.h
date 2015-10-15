#pragma once

#include <map>
#include <string>
#include "Controller.h"

using namespace std;

class App
{
private:
  map<string, Controller*> m_controllers;
public:
  App();
  ~App();
};

