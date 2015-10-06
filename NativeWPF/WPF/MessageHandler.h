#pragma once

#include <Windows.h>
#include <map>

class MessageHandler {
  typedef void(MessageHandler::*MessageFunction)(WPARAM wParam, LPARAM lParam);

private:
  std::map<UINT, MessageFunction> m_msgFuncTbl;

protected:
  void AddMessageFunc(UINT msg, MessageFunction func) {
    m_msgFuncTbl[msg] = func;
  }
};