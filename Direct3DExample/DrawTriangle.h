#pragma once

#include <d3dcompiler.h>
#include "D3DFramework.h"

#pragma comment (lib, "d3dcompiler.lib")

class DrawTriangle : public D3DFramework
{
    struct VERTEX
    {
        FLOAT X, Y, Z;
        FLOAT Color[4];
    };

    Microsoft::WRL::ComPtr<ID3D11InputLayout>           mspInputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer>                mspVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11VertexShader>          mspVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>           mspPixelShader;
    
public:
    void Initialize(HINSTANCE hInstance, int width = 800, int height = 600) override;
    void Destroy() override;
    
private:
    void InitTriangle();
    void InitPipeline();

protected:
    void Render() override;
};

//0. Vertex Shader, Pixel Shader
//    HLSL (High Level Shader Language)
//    nVidia cG : 비슷
//
//1. Vertex 구조 생성
//   Position (x, y, z)
//   Color (r, g, b, a)
//
//    + Normal, UV
//
//2. Vertex Buffer
//    Rame <->VRAM
//
//3. IA(Input Assembly) - 1번 정보 알려 줌
//
//4. Render PipeLine에 Vertex Buffer를 흘려 보내면 -> RenderTarget에 그려짐




// 삼각형 중간쯤은 r x 0.5 + g x 0.5 식으로 섞여짐 
// 고러드 셰이딩 (Gouraud Shading) - DirectX 기본값 