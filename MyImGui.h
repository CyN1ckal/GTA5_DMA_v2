#pragma once

class MyImGui
{
private: /* DirectX and Windows Variables */
	static inline ID3D11Device* g_pd3dDevice = nullptr;
	static inline ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
	static inline IDXGISwapChain* g_pSwapChain = nullptr;
	static inline bool g_SwapChainOccluded = false;
	static inline UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
	static inline ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
	static inline HWND g_hwnd = 0;
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static inline WNDCLASSEXW wc = { sizeof(WNDCLASSEXW), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };

private: /* DirectX and ImGui Wrappers */
	static bool CreateDeviceD3D(HWND hWnd);
	static void CleanupDeviceD3D();
	static void CreateRenderTarget();
	static void CleanupRenderTarget();
	static bool OnFrameStart();
	static bool OnFrameEnd();

public: /* Interface */
	static bool Initialize();
	static bool Close();
	static bool OnFrame();

public: /* Fonts */
	static inline ImFont* m_pIBMPlexMono_16 = nullptr;
	static inline ImFont* m_pIBMPlexMono_24 = nullptr;

private: /* ImGui Windows */
	static bool FeaturesWindow();

};