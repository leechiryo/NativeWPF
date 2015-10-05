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
      MainWindow app;

      Label * pLabel = app.CreateSubElement<Label>();
      pLabel->SetPosition(80, 100, 650, 150);
      pLabel->SetText(L"ラベルからのあいさつ！");
      pLabel->SetColor(0x656565);
      pLabel->SetFont(L"Migu 1M",
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL, 24.0f);

      app.Update();

      app.RunMessageLoop();

      Element::Uninitialize();
    }

    CoUninitialize();
  }

  return 0;
}
