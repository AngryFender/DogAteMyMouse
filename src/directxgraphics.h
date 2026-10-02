#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <dxgi.h>
#include <d3d11.h>
#include <vector>
#include "entity.h"
#include <Windows.h>

class DirectXGraphics{

public:
    DirectXGraphics(const OverlayContext& context) : context_(context), clear_color_(0.0f, 0.0f, 0.0f, 0.00f)
    {
    }

    DirectXGraphics(const DirectXGraphics&) = delete;
    DirectXGraphics& operator=(const DirectXGraphics&) = delete;

    DirectXGraphics(DirectXGraphics &&other) noexcept: clear_color_(other.clear_color_)
    {
        g_pd3dDevice = std::exchange(other.g_pd3dDevice, nullptr);
        g_pd3dDeviceContext = std::exchange(other.g_pd3dDeviceContext, nullptr);
        g_pSwapChain = std::exchange(other.g_pSwapChain, nullptr);
        g_mainRenderTargetView = std::exchange(other.g_mainRenderTargetView, nullptr);
    }

    DirectXGraphics& operator=(DirectXGraphics&& other) noexcept{
        if (this != &other) {

            if (g_pd3dDevice != nullptr) {
                ::ImGui_ImplDX11_Shutdown();
                ::ImGui_ImplWin32_Shutdown();
                ImGui::DestroyContext();
            }

            CleanupDeviceD3D();
            clear_color_ = other.clear_color_;
            g_pd3dDevice = std::exchange(other.g_pd3dDevice, nullptr);
            g_pd3dDeviceContext = std::exchange(other.g_pd3dDeviceContext, nullptr);
            g_pSwapChain = std::exchange(other.g_pSwapChain, nullptr);
            g_mainRenderTargetView = std::exchange(other.g_mainRenderTargetView, nullptr);
        }
        return *this;
    }

    ~DirectXGraphics() {
        if (g_pd3dDevice) {
            ::ImGui_ImplDX11_Shutdown();
            ::ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
        }
        CleanupDeviceD3D();
    }
    
    void init() {
        ::SetProcessDPIAware();
        float main_scale = ::ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0,0 }, MONITOR_DEFAULTTOPRIMARY));

        MARGINS margins = { -1, -1, -1,-1 };
        ::DwmExtendFrameIntoClientArea(context_.hWnd, &margins);
        ::SetLayeredWindowAttributes(context_.hWnd, 0, 255, LWA_ALPHA);

        if (!CreateDeviceD3D(context_.hWnd)) {
            CleanupDeviceD3D();
            assert(false && "Couldn't create Device D3D");
        }

        //setup Dear ImGui contexnt
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        //setup Dear ImGui style
        ImGui::StyleColorsLight();

        //setup scaling
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);
        style.FontScaleDpi = main_scale;

        //setup platform/renderer backends
        ::ImGui_ImplWin32_Init(context_.hWnd);
        ::ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    }

    void render_frame(const std::vector<std::pair<float, float>>& coordinates, const std::vector<Key>& keys) {

        //start new frame
        ::ImGui_ImplDX11_NewFrame();
        ::ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        //Bypass the ImGUI windows system and get the background canvas
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

       // Get the current font
        ImFont* current_font = ImGui::GetFont();
        float front_font_size = ImGui::GetFontSize() * 1.25f;
        float back_font_size = ImGui::GetFontSize() * 1.30f;
        float offset = 1.0f;
        auto shadow_color = IM_COL32(30, 30, 30, 255);
        auto front_color = IM_COL32(255, 0, 255, 255);
        
        for (int i = 0; i < coordinates.size(); ++i)
        {
            const auto& point = coordinates[i];
            const auto& key = keys[i];

            draw_list->AddText(
                current_font,
                back_font_size,
                ImVec2(point.first - offset, point.second),
                shadow_color,
                key.data(), key.data() + 2
            );

            draw_list->AddText(
                current_font,
                back_font_size,
                ImVec2(point.first + offset, point.second),
                shadow_color,
                key.data(), key.data() + 2
            );

            draw_list->AddText(
                current_font,
                back_font_size,
                ImVec2(point.first, point.second + offset),
                shadow_color,
                key.data(), key.data() + 2
            );

            draw_list->AddText(
                current_font,
                back_font_size,
                ImVec2(point.first, point.second - offset),
                shadow_color,
                key.data(), key.data() + 2
            );

            draw_list->AddText(
                current_font,
                front_font_size,
                ImVec2(point.first, point.second),
                front_color,
                key.data(), key.data() + 2
            );
        }

        //rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color_.x * clear_color_.w, clear_color_.y * clear_color_.w, clear_color_.z * clear_color_.w, clear_color_.w };

        //point the directX commander at the canvas
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);

        //wipe the canvas clean with the clean color
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);

        //pain the ImGui vertex data onto the canvas
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        //Enable VSync
        HRESULT hr = g_pSwapChain->Present(1, 0);
    }

    void showOverlay(bool show) const
    {
        int mode = show ? SW_SHOWDEFAULT : SW_HIDE;
        ::ShowWindow(context_.hWnd, mode);
        ::UpdateWindow(context_.hWnd);
    }

    void refresh() const {
        //GetMessage will see it, wake up, and move down to the DirectX render loop.
        //Places (posts) a message in the message queue associated with the thread that created the specified window and returns without waiting for the thread to process the message.
        ::PostMessage(context_.hWnd, WM_NULL, 0, 0);
    }

private:
    OverlayContext context_;

    ImVec4 clear_color_;

    //allocate memory and create resources in the gpu
    ID3D11Device* g_pd3dDevice = nullptr;

    //issues rendering commands
    ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;

    //holds the front and back bufferes and swaps them to the monitor
    IDXGISwapChain* g_pSwapChain = nullptr;

    //canvas; memory address on the gpu where the final image is painted 
    ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

    bool CreateDeviceD3D(HWND hWnd)
    {
        // Setup swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 1;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
        if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
            res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
        if (res != S_OK)
            return false;

        CreateRenderTarget();
        return true;
    }

    void CleanupDeviceD3D()
    {
        CleanupRenderTarget();
        if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
        if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
        if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    }

    void CreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer;
        g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
        pBackBuffer->Release();
    }

    void CleanupRenderTarget()
    {
        if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
    }

};