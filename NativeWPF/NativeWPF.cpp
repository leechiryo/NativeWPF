// NativeWPF.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "WPF\MainWindow.h"
#include "WPF\Label.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPTSTR    lpCmdLine,
  _In_ int       nCmdShow)
{
  HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

  if (SUCCEEDED(CoInitialize(NULL))) {
    if (SUCCEEDED(Element::Initialize())) {
      DemoApp app;

      app.Initialize();

      Label * pLabel = app.CreateSubElement<Label>();
      pLabel->SetPosition(80, 100, 300, 150);
      pLabel->SetText(L"ラベルからのあいさつ！");
      pLabel->SetColor(0x12a722);

      app.Update();

      app.RunMessageLoop();

      Element::Uninitialize();
    }

    CoUninitialize();
  }

  return 0;
}
