// NativeWPF.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "WPF\MainWindow.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPTSTR    lpCmdLine,
  _In_ int       nCmdShow)
{
  HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

  if (SUCCEEDED(CoInitialize(NULL))) {
    if (SUCCEEDED(Element::Initialize())) {
      DemoApp app;

      if (SUCCEEDED(app.Initialize())) {
        app.RunMessageLoop();
      }

      Element::Uninitialize();
    }

    CoUninitialize();
  }

  return 0;
}
