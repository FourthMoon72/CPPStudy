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






//1. 디바이스(Device) 생성
//    1-1. Device Context
//
//2. SwapChain 생성
//    2-1. BackBuffer(그리기), FrontBuffer(모니터)
//
//3. Render Target을 BackBuffer로 지정
//    ->BackBuffer를 RenderTarget으로 지정
//    ->RenderTargetView 같이 생성
//4. DepthBuffer 생성 (DepthStencil 같이 생성)
//
// 5. RenderTarget, DepthStencil을 파이프라인에 교체
// 
//6. Viewport 생성 (화면 일부분)
//  ->Rasterizer Stage에 연결
//7. 그리기
//    BackBuffer->FrontBuffer로 변경(화면에 그림 그려짐)
//      배경색 지우기

//------------------------------------------------------------------------------
/*창모드<->전체화면
    1. 해상도(화면크기) 변경
    2. SwapChain 옵션을 수정
    3. 게임 종료시 전체화면 해제
 -----------------------------------------------------------------------------------
해상도 변경
1. SwapChain 크기 변경
    1-1. 렌더타깃 변경(Front, Back 버퍼 크기가 변경)
2. Depth-Stencil 다시 생성
    2-1. Depth-Stencil View 다시 생성
 3. Viewport 변경 
  
  */


//========================================================================================


//  윈도우 생성
//        윈도우 클래스 등록
//        윈도우 생성
//        메세지 루프(이벤트 반복)
//        윈도우 프로시져
//
//  DirectX
//        스왑체인 (후면 버퍼)
//        렌더타켓 -> 후면버퍼
//        DepthStencil 생성
//        뷰포트 지정
//        Pipeline 그리기 (Present)
//
//  삼각형
//        버텍스 버퍼
//        IA Stage에 연결
//                LayOut 생성
//        Vertex Shader, Pixel Shader 
//                Compile 필요
//        Pipeline 몇개의 버텍스를 그릴건지 
        