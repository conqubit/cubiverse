#include "stdafx.h"
#include "System.h"

extern int width;
extern int height;

HRESULT r;

Direct3D::Direct3D() {
    ZeroMemory(this, sizeof(Direct3D));
}

Direct3D::~Direct3D() {
}

bool Direct3D::Init() {
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroStruct(scd);

    scd.BufferCount = 1;
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = System::hWindow;
    scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    r = D3D10CreateDeviceAndSwapChain(nullptr, D3D10_DRIVER_TYPE_HARDWARE, nullptr, 0, D3D10_SDK_VERSION, &scd, &swapChain, &device);
	if (FAILED(r)) {
		return false;
	}

	initRenderTarget();
	initViewPort();

	D3D10_BLEND_DESC blendStateDescription;
	ZeroStruct(blendStateDescription);
	blendStateDescription.BlendEnable[0] = true;
	blendStateDescription.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	blendStateDescription.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	blendStateDescription.BlendOp = D3D10_BLEND_OP_ADD;
	blendStateDescription.SrcBlendAlpha = D3D10_BLEND_ONE;
	blendStateDescription.DestBlendAlpha = D3D10_BLEND_ZERO;
	blendStateDescription.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	blendStateDescription.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	D3D10_SAMPLER_DESC sampDesc;
    ZeroStruct(sampDesc);
    sampDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D10_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D10_FLOAT32_MAX;
    r = device->CreateSamplerState(&sampDesc, &samp);

	if (FAILED(r)) {
		return true;
	}

	device->CreateBlendState(&blendStateDescription, &BlendState);

	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	device->OMSetBlendState(BlendState, blendFactor, 0xffffffff);
	return true;

}

void Direct3D::Shutdown() {
	rasterState->Release();
	depthStencilView->Release();
	depthStencilBuffer->Release();
	renderTarget->Release();
    device->Release();
    swapChain->Release();
}

void Direct3D::ClearDepth() {
    device->ClearDepthStencilView(depthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::BeginScene() {
    device->ClearRenderTargetView(renderTarget, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	ClearDepth();
}

void Direct3D::EndScene() {
	swapChain->Present(0, 0);
}

bool Direct3D::initRenderTarget() {
    ID3D10Texture2D *backbuffer;
	r = swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&backbuffer);
	if (FAILED(r)) return false;

	r = device->CreateRenderTargetView(backbuffer, nullptr, &renderTarget);
	if (FAILED(r)) return false;

    backbuffer->Release();

	// Create depth stencil texture
    D3D10_TEXTURE2D_DESC descDepth;
    ZeroStruct(descDepth);
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D10_USAGE_DEFAULT;
    descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

	r = device->CreateTexture2D( &descDepth, nullptr, &depthStencilBuffer );
    if (FAILED(r)) return false;

    // Create the depth stencil view
    D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroStruct(descDSV);
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2DMS;

	r = device->CreateDepthStencilView( depthStencilBuffer, &descDSV, &depthStencilView );
    if (FAILED(r)) return false;

    device->OMSetRenderTargets( 1, &renderTarget, depthStencilView );

	// Setup the raster description which will determine how and what polygons will be drawn.
	D3D10_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D10_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D10_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	r = device->CreateRasterizerState(&rasterDesc, &rasterState);
	if (FAILED(r)) return false;

	// Now set the rasterizer state.
	device->RSSetState(rasterState);

	return true;
}

bool Direct3D::initViewPort() {
	D3D10_VIEWPORT viewport;
    ZeroStruct(viewport);

	viewport.Width = (float)width;
    viewport.Height = (float)height;
	viewport.MinDepth = 0.0;
	viewport.MaxDepth = 1.0;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    device->RSSetViewports(1, &viewport);

	return true;
}