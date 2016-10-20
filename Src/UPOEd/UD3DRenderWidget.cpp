#include "UD3DRenderWidget.h"
#include "../UPOEngine/GFXCore/URenderer.h"

namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	D3DRenderWidget::D3DRenderWidget(QWidget* parent /*= nullptr*/)
		: QWidget(parent)
	{
		setAttribute(Qt::WA_PaintOnScreen);
		setAttribute(Qt::WA_NativeWindow);
		CreateDevice();
	}
	//////////////////////////////////////////////////////////////////////////
	D3DRenderWidget::~D3DRenderWidget()
	{

	}

	void D3DRenderWidget::resizeEvent(QResizeEvent* evt)
	{
		ULOG_MESSAGE("resizing %d %d", width(), height());
		mContext->Resize(width(), height());
	}

	void D3DRenderWidget::paintEvent(QPaintEvent* evt)
	{
		Render();
	}

	//////////////////////////////////////////////////////////////////////////
	bool D3DRenderWidget::CreateDevice()
	{
		// 		swapChainDesc_.BufferCount = 1;
		// 		swapChainDesc_.BufferDesc.Width = width();
		// 		swapChainDesc_.BufferDesc.Height = height();
		// 		swapChainDesc_.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		// 		swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		// 		swapChainDesc_.SampleDesc.Count = 4;
		// 		swapChainDesc_.SampleDesc.Quality = 0;
		// 		swapChainDesc_.Windowed = true;
		// 		swapChainDesc_.OutputWindow = winId();
		// 		swapChainDesc_.BufferDesc.RefreshRate.Numerator = 60;
		// 		swapChainDesc_.BufferDesc.RefreshRate.Denominator = 1;

		mContext = GFXContext::CreateContext();
		GFXContext::InitParam ip;
		ip.mWidth = width();
		ip.mHeight = height();
		ip.mWindowHandle = reinterpret_cast<void*>(winId());

		mContext->Init(ip);

		Renderer::Get()->Init(mContext);
		
		return true;
	};

	void D3DRenderWidget::Render()
	{
		
	}

}