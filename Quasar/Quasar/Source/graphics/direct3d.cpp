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

// Initializes D3D object.
bool Direct3D::Init() {
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroStruct(scd);

    scd.BufferCount = 1;
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = System::hWindow;
    scd.SampleDesc.Count = 8;
	scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	r = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
			D3D11_SDK_VERSION, &scd, &swapChain, &device, nullptr, &deviceContext);
	if (FAILED(r)) {
		return false;
	}

	initRenderTarget();
	initViewPort();

	D3D11_BLEND_DESC blendStateDescription;
	ZeroStruct(blendStateDescription);
	blendStateDescription.RenderTarget[0].BlendEnable = true;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	D3D11_SAMPLER_DESC sampDesc;
    ZeroStruct(sampDesc);
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
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

	deviceContext->OMSetBlendState(BlendState, blendFactor, 0xffffffff);
	return true;

}

void Direct3D::Shutdown() {
	rasterState->Release();
	depthStencilView->Release();
	depthStencilBuffer->Release();
	renderTarget->Release();
    deviceContext->Release();
    device->Release();
    swapChain->Release();
}

void Direct3D::BeginScene() {
    deviceContext->ClearRenderTargetView(renderTarget, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::EndScene() {
	swapChain->Present(0, 0);
}

bool Direct3D::initRenderTarget() {
    ID3D11Texture2D *backbuffer;
	r = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
	if (FAILED(r)) return false;

	r = device->CreateRenderTargetView(backbuffer, nullptr, &renderTarget);
	if (FAILED(r)) return false;

    backbuffer->Release();

	// Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroStruct(descDepth);
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 8;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

	r = device->CreateTexture2D( &descDepth, nullptr, &depthStencilBuffer );
    if (FAILED(r)) return false;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroStruct(descDSV);
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	r = device->CreateDepthStencilView( depthStencilBuffer, &descDSV, &depthStencilView );
    if (FAILED(r)) return false;

    deviceContext->OMSetRenderTargets( 1, &renderTarget, depthStencilView );

	// Setup the raster description which will determine how and what polygons will be drawn.
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	r = device->CreateRasterizerState(&rasterDesc, &rasterState);
	if (FAILED(r)) return false;

	// Now set the rasterizer state.
	deviceContext->RSSetState(rasterState);

	return true;
}

bool Direct3D::initViewPort() {
	D3D11_VIEWPORT viewport;
    ZeroStruct(viewport);

	viewport.Width = (float)width;
    viewport.Height = (float)height;
	viewport.MinDepth = 0.0;
	viewport.MaxDepth = 1.0;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    deviceContext->RSSetViewports(1, &viewport);

	return true;
}