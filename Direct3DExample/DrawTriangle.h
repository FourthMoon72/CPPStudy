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
//    nVidia cG : ���
//
//1. Vertex ���� ����
//   Position (x, y, z)
//   Color (r, g, b, a)
//
//    + Normal, UV
//
//2. Vertex Buffer
//    Rame <->VRAM
//
//3. IA(Input Assembly) - 1�� ���� �˷� ��
//
//4. Render PipeLine�� Vertex Buffer�� ��� ������ -> RenderTarget�� �׷���




// �ﰢ�� �߰����� r x 0.5 + g x 0.5 ������ ������ 
// ���� ���̵� (Gouraud Shading) - DirectX �⺻�� 