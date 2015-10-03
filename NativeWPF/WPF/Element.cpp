#include "Element.h"

ID2D1Factory* Element::s_pDirect2dFactory = nullptr;
ID2D1HwndRenderTarget* Element::s_pRenderTarget = nullptr;
IDWriteFactory* Element::s_pDWriteFactory = nullptr;

void Element::Initialize()
{
}
