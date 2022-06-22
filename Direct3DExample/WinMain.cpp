#include "DrawSquare.h"


int WINAPI WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    DrawSquare myFramework;

    myFramework.Initialize(hInstance);
    myFramework.GameLoop();
    myFramework.Destroy();


    return 0;
}






//1. ����̽�(Device) ����
//    1-1. Device Context
//
//2. SwapChain ����
//    2-1. BackBuffer(�׸���), FrontBuffer(�����)
//
//3. Render Target�� BackBuffer�� ����
//    ->BackBuffer�� RenderTarget���� ����
//    ->RenderTargetView ���� ����
//4. DepthBuffer ���� (DepthStencil ���� ����)
//
// 5. RenderTarget, DepthStencil�� ���������ο� ��ü
// 
//6. Viewport ���� (ȭ�� �Ϻκ�)
//  ->Rasterizer Stage�� ����
//7. �׸���
//    BackBuffer->FrontBuffer�� ����(ȭ�鿡 �׸� �׷���)
//      ���� �����

//------------------------------------------------------------------------------
/*â���<->��üȭ��
    1. �ػ�(ȭ��ũ��) ����
    2. SwapChain �ɼ��� ����
    3. ���� ����� ��üȭ�� ����
 -----------------------------------------------------------------------------------
�ػ� ����
1. SwapChain ũ�� ����
    1-1. ����Ÿ�� ����(Front, Back ���� ũ�Ⱑ ����)
2. Depth-Stencil �ٽ� ����
    2-1. Depth-Stencil View �ٽ� ����
 3. Viewport ���� 
  
  */


//========================================================================================


//  ������ ����
//        ������ Ŭ���� ���
//        ������ ����
//        �޼��� ����(�̺�Ʈ �ݺ�)
//        ������ ���ν���
//
//  DirectX
//        ����ü�� (�ĸ� ����)
//        ����Ÿ�� -> �ĸ����
//        DepthStencil ����
//        ����Ʈ ����
//        Pipeline �׸��� (Present)
//
//  �ﰢ��
//        ���ؽ� ����
//        IA Stage�� ����
//                LayOut ����
//        Vertex Shader, Pixel Shader 
//                Compile �ʿ�
//        Pipeline ��� ���ؽ��� �׸����� 
        