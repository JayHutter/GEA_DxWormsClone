//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <time.h>

//Scarle Headers
#include "DrawData.h"
#include "DrawData2D.h"
#include "ObjectList.h"
#include "GameData.h"
#include "GameState.h"
#include "RenderTarget.h"

//DXTK Headers
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "CommonStates.h"

//Objects
#include "Worm.h"
#include "Stage.h"

extern void ExitGame();

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept :
    m_window(nullptr),
    m_outputWidth(1280),
    m_outputHeight(720),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND _window, int _width, int _height)
{
    m_window = _window;
    m_outputWidth = std::max(_width, 1);
    m_outputHeight = std::max(_height, 1);

    CreateDevice();

    CreateResources();

    //set up keyboard and mouse system
    //documentation here: https://github.com/microsoft/DirectXTK/wiki/Mouse-and-keyboard-input
    m_keyboard = std::make_unique<Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(_window);
    m_mouse->SetMode(Mouse::MODE_ABSOLUTE);
    ShowCursor(true);

    //seed the random number generator
    srand((UINT)time(NULL));

    //set up systems for 2D rendering
    m_DD2D = new DrawData2D();
    m_DD2D->m_Sprites.reset(new SpriteBatch(m_d3dContext.Get()));
    m_DD2D->m_Font.reset(new SpriteFont(m_d3dDevice.Get(), L"..\\Fonts\\italic.spritefont"));
    m_states = new CommonStates(m_d3dDevice.Get());

    //set audio system
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif
    m_audioEngine = std::make_unique<AudioEngine>(eflags);

    //set up systems for 3D rendering
    
    //set up DirectXTK Effects system
    m_fxFactory = new EffectFactory(m_d3dDevice.Get());
    //Tell the fxFactory to look to the correct build directory to pull stuff in from
    ((EffectFactory*)m_fxFactory)->SetDirectory(L"..\\Models");
    //init render system for VBGOs
    VBGO::Init(m_d3dDevice.Get());

    //find how big my window is to correctly calculate my aspect ratio
    float AR = (float)_width / (float)_height;
        
    /*
    //create a base camera
    m_cam = new Camera(0.25f * XM_PI, AR, 1.0f, 10000.0f, Vector3::UnitY, Vector3::Zero);
    m_cam->SetPos(Vector3(0.0f, 200.0f, 200.0f));
    m_GameObjects.push_back(m_cam);

    //create a base light
    m_light = new Light(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
    m_GameObjects.push_back(m_light);

    //add a secondary camera
    //m_TPScam = new TPSCamera(0.25f * XM_PI, AR, 1.0f, 10000.0f, pPlayer, Vector3::UnitY, Vector3(0.0f, 10.0f, 50.0f));
    //m_GameObjects.push_back(m_TPScam);

    //create DrawData struct and populate its pointers
    m_DD = new DrawData;
    m_DD->m_pd3dImmediateContext = nullptr;
    m_DD->m_states = m_states;
    m_DD->m_cam = m_cam;
    m_DD->m_light = m_light;
    */

    //create GameData struct and populate its pointers
    m_GD = new GameData;
    m_GD->m_GS = GS_PLAY_MAIN_CAM;

    //Test Sounds
    //Loop* loop = new Loop(m_audioEngine.get(), "NightAmbienceSimple_02");
    //loop->SetVolume(0.1f);
    //loop->Play();
    //m_Sounds.push_back(loop);

    //TestSound* TS = new TestSound(m_audioEngine.get(), "Explo1");
    //m_Sounds.push_back(TS);

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:

    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);

    //create RenderTarget for Terrain
    //TODO: What size do you REALLY need for this?
    m_terrain = new RenderTarget(m_d3dDevice.Get(), m_outputWidth, m_outputHeight);

    //m_level = new LevelManager(m_d3dDevice.Get());
    ////m_level->SetupLevel("test_stage2", 4, m_d3dDevice.Get());
    //m_level->SetupLevel("test_stage2", 2, 2);

   auto m_menu = new Menu(m_d3dDevice.Get());
   m_menu->SetupMenu();


    //auto level = new LevelManager(m_d3dDevice.Get());
    //level->SetupLevel("test_stage2", 2, 2);

    m_stack.push_back(m_menu);

    //alpha = new ImageGO2D("alpha_test", m_d3dDevice.Get());
    //alpha->SetPos(Vector2(600, 600));
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& _timer)
{
    float elapsedTime = float(_timer.GetElapsedSeconds());
    m_GD->m_dt = elapsedTime;

    //this will update the audio engine but give us chance to do somehting else if that isn't working
    if (!m_audioEngine->Update())
    {
        if (m_audioEngine->IsCriticalError())
        {
            // We lost the audio device!
        }
    }
    else
    {
        //update sounds playing
        for (list<Sound*>::iterator it = m_Sounds.begin(); it != m_Sounds.end(); it++)
        {
            (*it)->Tick(m_GD);
        }
    }

    ReadInput();
    //upon space bar switch camera state
    //see docs here for what's going on: https://github.com/Microsoft/DirectXTK/wiki/Keyboard

    //update all objects  TODO: Move to manager
    /*
    for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
    {
        (*it)->Tick(m_GD);
    }
    */
    for (list<GameObject2D*>::iterator it = m_GameObjects2D.begin(); it != m_GameObjects2D.end(); it++)
    {
        (*it)->Tick(m_GD);
    }

    if (m_stack.size() > 0)
    {
        m_stack.back()->Update(m_GD, m_terrain, m_d3dContext.Get());
        LoadLevel();
        if (m_stack.back()->DeleteScreen())
        {
            delete m_stack.back();
            m_stack.pop_back();
        }
    }
    else
    {
        ExitGame();
    }

    elapsedTime;
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    // Draw sprite batch stuff
    m_DD2D->m_Sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
    for (list<GameObject2D*>::iterator it = m_GameObjects2D.begin(); it != m_GameObjects2D.end(); it++)
    {
        (*it)->Draw(m_DD2D);
    }
    m_DD2D->m_Sprites->End();

    if (m_stack.size() > 0)
    {
        m_stack.back()->Draw(m_DD2D, m_terrain, m_d3dContext.Get(), m_states);
    }

    //drawing text screws up the Depth Stencil State, this puts it back again!
    m_d3dContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
    
    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    if (m_stack.size() > 0)
    {
        m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), m_stack.back()->GetBG());
    }
    else
    {
        m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::SkyBlue);
    }

    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
     HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int _width, int _height)
{
   //m_outputWidth = std::max(_width, 1);
   //m_outputHeight = std::max(_height, 1);
   //
   //CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& _width, int& _height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    _width = 1280;
    _height = 720;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    DX::ThrowIfFailed(D3D11CreateDevice(
        nullptr,                            // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        device.ReleaseAndGetAddressOf(),    // returns the Direct3D device created
        &m_featureLevel,                    // returns feature level of device created
        context.ReleaseAndGetAddressOf()    // returns the device immediate context
        ));

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(device.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    DX::ThrowIfFailed(device.As(&m_d3dDevice));
    DX::ThrowIfFailed(context.As(&m_d3dContext));

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory2> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        // Create a descriptor for the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = backBufferWidth;
        swapChainDesc.Height = backBufferHeight;
        swapChainDesc.Format = backBufferFormat;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = backBufferCount;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = TRUE;

        // Create a SwapChain from a Win32 window.
        DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
            m_d3dDevice.Get(),
            m_window,
            &swapChainDesc,
            &fsSwapChainDesc,
            nullptr,
            m_swapChain.ReleaseAndGetAddressOf()
            ));

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain.Reset();
    m_d3dContext.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}

void Game::ReadInput()
{
    m_GD->m_KBS = m_keyboard->GetState();
    m_GD->m_KBS_tracker.Update(m_GD->m_KBS);
    //quit game on hiting escape
    //if (m_GD->m_KBS.Escape)
    //{
    //    ExitGame();
    //}

    m_GD->m_MS = m_mouse->GetState();

    //lock the cursor to the centre of the window
    RECT window;
    GetWindowRect(m_window, &window);
  //  SetCursorPos((window.left + window.right) >> 1, (window.bottom + window.top) >> 1);
}

void Game::LoadLevel()
{
    auto new_screen = m_stack.back()->Load();

    if (new_screen)
    {
        m_stack.push_back(new_screen);
    }
}