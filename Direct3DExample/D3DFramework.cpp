#include <sstream>
#include "D3DFramework.h"

void D3DFramework::CalculateFPS()
{
    static int frameCount{};
    static float timeElapsed{};
    
    frameCount++;

    if (mTimer.TotalTime() - timeElapsed >= 1.0f)
    {
        float fps = static_cast<float>(frameCount);
        float mspf = 1000.0f / fps;

        std::wostringstream oss;

        oss.precision(6);
        oss << mTitleText << L" - " <<
            L"FPS: " << fps <<
            L", Frame Time : " << mspf << L"(ms)";

        SetWindowText(mHwnd, oss.str().c_str());

        frameCount = 0;
        timeElapsed += 1.0f;       
    }
}

void D3DFramework::InitWindow(HINSTANCE hInstance)
{
        WNDCLASSEX wc;

        ZeroMemory(&wc, sizeof(WNDCLASSEX));

        mInstance = hInstance;

        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpszClassName = CLASSNAME.c_str();
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = 0;
        wc.lpfnWndProc = WindowProc;
        wc.cbSize = sizeof(WNDCLASSEX);

        if (!RegisterClassEx(&wc))
        {
            MessageBox(nullptr, L"Failed to register window class!", L"Error",
                MB_ICONEXCLAMATION | MB_OK);
            return ;
        }

        RECT wr{ 0, 0, mScreenWidth, mScreenHeight };
        AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

        mTitleText = TITLENAME;

        mHwnd = CreateWindowEx(NULL,
            CLASSNAME.c_str(),
            mTitleText.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            wr.right - wr.left,
            wr.bottom - wr.top,
            NULL,
            NULL,
            hInstance,
            NULL);

        if (mHwnd == nullptr)
        {
            MessageBox(nullptr, L"Failed to create window class!", L"Error",
                MB_ICONEXCLAMATION | MB_OK);
            return;
        }
        SetWindowLongPtr(mHwnd, GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(this));


        ShowWindow(mHwnd, SW_SHOW);
        SetForegroundWindow(mHwnd); //���� Ŭ���ϸ� â�� �� ������
        SetFocus(mHwnd); //���� �ո��̿��� �Է¹��� �� �ִ� ���·� ����
        UpdateWindow(mHwnd);
}

void D3DFramework::InitD3D()
{
    DXGI_SWAP_CHAIN_DESC scd;

    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferCount = 1;                                //BackBuffer�� ����
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //PixelFormat
    scd.BufferDesc.Width = mScreenWidth;
    scd.BufferDesc.Height = mScreenHeight;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  //BackBuffer�� RenderTarget���� ������ ����
    scd.OutputWindow = mHwnd;                           //� �����쿡�� �׸��� ����Ұž�?
    scd.SampleDesc.Count = 1;                           //1xMSAA - Anti-Alisasing ����
    scd.Windowed = TRUE;                                //â���
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


    //2.

    D3D11CreateDeviceAndSwapChain(
        NULL,                                        //Adaptor Pointer
        D3D_DRIVER_TYPE_HARDWARE,                    //�������� �ϵ����(�׷��� ī��)
        NULL,                                        //����Ʈ���� ������ ����
        NULL,                                        //�ɼ�
        NULL,                                        //const * (�迭 �Ѱ��� �� ���� ǥ��)///// D3D_FEATURE_LEVEL �迭 - �ּ� ���
        0,                                           //�迭 ũ�� (������ �迭�� �� �Ѱ������� ũ��� 0)
        D3D11_SDK_VERSION,                           //3DK
        &scd,                                        //const * (�迭 �ƴ�) DXGI_SWAP_CHAIN_DESC ����ü������ *
        mspSwapChain.ReleaseAndGetAddressOf(),       //������ SwapChain �������̽��� �����Ͱ� ��ȯ
        mspDevice.ReleaseAndGetAddressOf(),          //������ Device �������̽�
        NULL,                                        //�׷���ī�尡 �����ϴ� DX ���� ���
        mspDeviceContext.ReleaseAndGetAddressOf()    //������ DeviceContext �������̽�
    );

    OnResize();
}

void D3DFramework::OnResize()
{
    // Swap Chain ũ�� ����
    ID3D11RenderTargetView* nullView[] = { nullptr };
    mspDeviceContext->OMSetRenderTargets(_countof(nullView), nullView, nullptr);

    mspRenderTargetView.Reset();
    mspDepthStencilView.Reset();
    mspRenderTarget.Reset();
    mspDepthStencil.Reset();

    mspDeviceContext->Flush();
    mspSwapChain->ResizeBuffers(
        0,
        mScreenWidth,
        mScreenHeight,
        DXGI_FORMAT_UNKNOWN,
        0
    );


    //  ����ü�� (Front, Back) <- Back �׸� ���� <- RenderTarget�� BackBuffer�� ����
    mspSwapChain->GetBuffer(0, IID_PPV_ARGS(mspRenderTarget.ReleaseAndGetAddressOf()));

    //  resource -> resource view ����
    mspDevice->CreateRenderTargetView(
        mspRenderTarget.Get(),
        nullptr,
        mspRenderTargetView.GetAddressOf());

    //Depth-Stencil Buffer
    CD3D11_TEXTURE2D_DESC t2d(
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        mScreenWidth,
        mScreenHeight,
        1,
        1,
        D3D11_BIND_DEPTH_STENCIL
    );

    mspDevice->CreateTexture2D(&t2d, nullptr, mspDepthStencil.ReleaseAndGetAddressOf());

    CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd(D3D11_DSV_DIMENSION_TEXTURE2D);

    mspDevice->CreateDepthStencilView(
        mspDepthStencil.Get(),
        &dsvd,
        mspDepthStencilView.ReleaseAndGetAddressOf()
    );

    // Output Merger�� Rendering Pipe�� ����
    mspDeviceContext->OMSetRenderTargets(
        1,
        mspRenderTargetView.GetAddressOf(),
        mspDepthStencilView.Get()
    );
    //Viewport
    CD3D11_VIEWPORT viewport(
        0.0f, 0.0f,
        static_cast<float>(mScreenWidth),
        static_cast<float>(mScreenHeight)
    );
    mspDeviceContext->RSSetViewports(1, &viewport);
}

void D3DFramework::RenderFrame()
{
    float clear_color[4]{ 0.0f, 0.2f, 0.4f, 1.0f }; // rgba UNORM
    mspDeviceContext->ClearRenderTargetView(mspRenderTargetView.Get(), clear_color);
    mspDeviceContext->ClearDepthStencilView(
        mspDepthStencilView.Get(),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
        1.0f,
        0
    );

    Render();

    mspSwapChain->Present(0, 0);
}

void D3DFramework::Initialize(HINSTANCE hInstance, int width, int height)
{
    mScreenWidth = width;
    mScreenHeight = height;
    mPaused = false;

    InitWindow(hInstance);
    InitD3D();
}

void D3DFramework::Destroy()
{
    mspSwapChain->SetFullscreenState(FALSE, nullptr);

    mspDepthStencilView.Reset();
    mspDepthStencil.Reset();
    mspRenderTarget.Reset();
    mspRenderTargetView.Reset();
    mspSwapChain.Reset();
    mspDeviceContext.Reset();
    mspDevice.Reset();


    DestroyWindow(mHwnd);
    UnregisterClass(CLASSNAME.c_str(), mInstance);
}

void D3DFramework::GameLoop()
{
    MSG msg{};

    mTimer.Start();

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                break;
            }
        }
        else
        {
            mTimer.Update();

            if (mPaused)
            {
                Sleep(100);
            }
            else
            {
                CalculateFPS();

                // Game Loop
                Update(mTimer.DeltaTime());
                RenderFrame();
            }
        }
    }
}

LRESULT D3DFramework::MessageHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_KEYDOWN:
            mInput.SetKeyDown(static_cast<unsigned int>(wParam));
            break;

        case WM_KEYUP:
            mInput.SetKeyUp(static_cast<unsigned int>(wParam));
            break;

        case WM_LBUTTONDOWN:
            mInput.SetKeyDown(VK_LBUTTON);
            break;

        case WM_LBUTTONUP:
            mInput.SetKeyUp(VK_LBUTTON);
            break;

        case WM_RBUTTONDOWN:
            mInput.SetKeyDown(VK_RBUTTON);
            break;

        case WM_RBUTTONUP:
            mInput.SetKeyDown(VK_RBUTTON);
            break;

        case WM_MOUSEMOVE:
            mInput.SetCursor(LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_ACTIVATE:
            if (LOWORD(lParam) == WA_INACTIVE)
            {
                mPaused = true;
                mTimer.Stop();
            }
            else
            {
                mPaused = false;
                mTimer.Resume();
            }
            break;

        case WM_PAINT:
            if (mResizing)
            {
                RenderFrame();
            }
            else
            {
                PAINTSTRUCT ps;
                BeginPaint(hWnd, &ps);
                EndPaint(hWnd, &ps);
            }
            break;

        case WM_ENTERSIZEMOVE:
            mPaused = true;
            mTimer.Stop();
            mResizing = true;
            return 0;
            break;

        case WM_SIZE:
            mScreenWidth = LOWORD(lParam);
            mScreenHeight = HIWORD(lParam);

            if (mspDevice)
            {
                if (wParam == SIZE_MINIMIZED)
                {
                    if (!mPaused)
                    {
                        mTimer.Stop();
                    }                
                    mPaused = true;
                    mMinimized = true;
                    mMaximized = false;
                }
                else if (wParam == SIZE_MAXIMIZED)
                {
                    mTimer.Resume();
                    mPaused = false;
                    mMinimized = false;
                    mMaximized = true;
                    OnResize();
                }
                else if (wParam == SIZE_RESTORED)
                {
                    if (mMinimized)
                    {
                        mPaused = false;
                        mTimer.Resume();
                        mMinimized = false;
                        OnResize();
                    }
                    else if (mMaximized)
                    {
                        mPaused = false;
                        mTimer.Resume();
                        mMaximized = false;
                        OnResize();
                    }
                    else if (mResizing)
                    {
                        //����ڰ� �巡�� ��
                    }
                    else
                    {
                        mPaused = false;
                        mTimer.Resume();
                        OnResize();
                    }
                }
                else
                {
                    //����ڰ� �巡�� ��
                }
            }
            return 0;
            break;

        case WM_EXITSIZEMOVE:
            mPaused = false;
            mTimer.Resume();
            mResizing = false;
            OnResize();
            return 0;
            break;


        case WM_GETMINMAXINFO:
            reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.x = 640;
            reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.y = 480;
            return 0;
            break;


        case WM_MENUCHAR:
            return MAKELRESULT(0, MNC_CLOSE);
            break;


        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
    
}

LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    D3DFramework* pFramework = reinterpret_cast<D3DFramework*>(
        GetWindowLongPtr(hWnd, GWLP_USERDATA)
        );

    return pFramework->MessageHandle(hWnd, message, wParam, lParam);

    
}
