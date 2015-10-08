// NativeWPF.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "WPF\MainWindow.h"
#include "WPF\Label.h"
#include "WPF\ClickableElement.h"

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

      Label * pLabel2 = app.CreateSubElement<Label>();
      pLabel2->SetPosition(80, 170, 650, 270);
      pLabel2->SetText(L"'Hello, world!' from the Label control.");
      pLabel2->SetColor(0x333399);
      pLabel2->SetFont(L"Source Code Pro",
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL, 16.0f);

      ClickableElement *clickbtn = app.CreateSubElement<ClickableElement>();
      clickbtn->SetPosition(80, 300, 100, 320);

      app.Update();

      app.RunMessageLoop();

      Element::Uninitialize();
    }

    CoUninitialize();
  }

  return 0;
}
