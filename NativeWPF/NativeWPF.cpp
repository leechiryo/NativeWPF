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

  if (SUCCEEDED(CoInitialize(NULL))){
    DemoApp app;

    if (SUCCEEDED(app.Initialize())){
      app.RunMessageLoop();
    }

    CoUninitialize();
  }

  return 0;
}
