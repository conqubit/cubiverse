#pragma once

#include <d3d10.h>
#include <d3dx10.h>
//#include <D3DCSX.h>
//#include <D3Dcompiler.h>

#pragma comment (lib, "d3d10.lib")
#pragma comment (lib, "d3dx10.lib")

class Direct3D {
public:
    Direct3D();
    ~Direct3D();

    bool Init();

    void Render();
    void BeginScene();
    void EndScene();
    void Shutdown();

    ID3D10Device* device;
    ID3D10Buffer* globalConstBuffer;

    IDXGISwapChain *swapChain;
    ID3D10RenderTargetView *renderTarget;

    ID3D10Texture2D* depthStencilBuffer;
    ID3D10DepthStencilView* depthStencilView;
    ID3D10RasterizerState* rasterState;
    ID3D10BlendState *BlendState;
    ID3D10SamplerState* samp;

private:
    bool initRenderTarget();
    bool initViewPort();
};