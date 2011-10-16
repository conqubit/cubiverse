#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <D3DCSX.h>
#include <D3Dcompiler.h>
#include "math.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

class Direct3D {
public:
	Direct3D();
	~Direct3D();

	bool init();

	void render();
	void beginScene();
	void endScene();
	void shutdown();

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11Buffer* globalConstBuffer;

	IDXGISwapChain *swapChain;
	ID3D11RenderTargetView *renderTarget;

	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;
	ID3D11BlendState *BlendState;
	ID3D11SamplerState* samp;

private:
	bool initRenderTarget();
	bool initViewPort();
};