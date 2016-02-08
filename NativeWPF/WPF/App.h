#pragma once

#include <map>
#include <string>
#include "Controller.h"
#include "View.h"

using namespace std;

class App
{
private:
  map<string, ControllerBase*> m_controllers;
  map<string, ViewBase*> m_views;
public:
  App();
  ~App();
};

