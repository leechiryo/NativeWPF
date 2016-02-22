#include "App.h"
#include <system_error>
#include "CommonFunction.h"
#include "ViewBase.h"

ID2D1Factory* App::s_pDirect2dFactory = nullptr;
IDWriteFactory* App::s_pDWriteFactory = nullptr;
double App::DPI_SCALE_X = 1.0f;
double App::DPI_SCALE_Y = 1.0f;

App::App()
{
  HRESULT hr = CoInitialize(NULL);
  if (!SUCCEEDED(hr)) {
    throw std::system_error(EINTR, std::system_category(), "COM environment is not initialized successfully.");
  }
  // create the d2d factory.
  hr = D2D1CreateFactory(
    D2D1_FACTORY_TYPE_SINGLE_THREADED,
    &s_pDirect2dFactory);

  if (!SUCCEEDED(hr)) {
    CoUninitialize();
    throw std::system_error(EINTR, std::system_category(), "Direct2D is not initialized successfully.");
  }

  // create the dwrite factory.
  hr = DWriteCreateFactory(
    DWRITE_FACTORY_TYPE_SHARED,
    __uuidof(IDWriteFactory),
    reinterpret_cast<IUnknown**>(&s_pDWriteFactory));

  if (!SUCCEEDED(hr)) {
    SafeRelease(s_pDirect2dFactory);
    CoUninitialize();
    throw std::system_error(EINTR, std::system_category(), "DirectWrite is not initialized successfully.");
  }

  float dpiX, dpiY;
  s_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);

  DPI_SCALE_X = dpiX / 96.0f;
  DPI_SCALE_Y = dpiY / 96.0f;
}

App::~App()
{
  SafeRelease(s_pDWriteFactory);
  SafeRelease(s_pDirect2dFactory);
  CoUninitialize();

  for (auto v : m_views) {
    if (v.second != nullptr) {
      delete v.second;
      v.second = nullptr;
    }
  }

  for (auto m : m_models) {
    if (m != nullptr) {
      delete m;
      m = nullptr;
    }
  }

  for (auto mc : m_modelCheckers) {
    if (mc != nullptr) {
      delete mc;
      mc = nullptr;
    }
  }
}

template <typename ViewType>
ViewType* App::GetView(string id)
{
  if (m_views.find(id) != m_views.end()) {
    ViewType* ret = dynamic_cast<ViewType*>(m_views[id]);
    if (ret == nullptr) return ret;
    else throw std::runtime_error("Type error! Cannot convert the view to the specified type. id = " + id);
  }
  return nullptr;
}


void App::UpdateViews() {
  for (auto mc : m_modelCheckers) {
    if (mc->ModelChanged()) {
      const void * pModel = mc->GetModelAddr();
      if (m_bindings.find(pModel) != m_bindings.end()) {
        for (auto v : m_bindings[pModel]) {
          v->Draw();
        }
      }
    }
  }
}
