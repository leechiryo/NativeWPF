#include "Element.h"

ID2D1Factory* Element::s_pDirect2dFactory = nullptr;
IDWriteFactory* Element::s_pDWriteFactory = nullptr;
float Element::DPI_SCALE_X = 1.0f;
float Element::DPI_SCALE_Y = 1.0f;