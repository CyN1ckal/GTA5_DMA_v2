#include "pch.h"

#include "MyImGui.h"

#pragma comment(lib, "d3d11.lib")

#include "Features.h"

#include "Fonts.h"

extern bool g_Alive;

bool MyImGui::OnFrame()
{
	if (!OnFrameStart())
		return 1;

	Fuser::OnFrame();

	WeaponInspector::OnFrame();
	WeaponEditor::OnFrame();

	VehicleInspector::OnFrame();
	VehicleEditor::OnFrame();

	MainMenu::Render();

	BlipInspector::OnFrame();

	OnFrameEnd();

	return 1;
}

bool MyImGui::Initialize()
{
	RegisterClassExW(&wc);
	g_hwnd = ::CreateWindowW(wc.lpszClassName, L"CyNickal's GTA5 Menu v2", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

	// Initialize Direct3D
	if (!CreateDeviceD3D(g_hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		throw std::runtime_error("CreateDeviceD3D Failed!");
	}

	// Show the window
	::ShowWindow(g_hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(g_hwnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

	ImFontConfig cfg;
	cfg.FontDataOwnedByAtlas = false;
	m_pIBMPlexMono_16 = io.Fonts->AddFontFromMemoryTTF(IBMPlexMono_Bold_Data, sizeof(IBMPlexMono_Bold_Data), 16, &cfg);
	m_pIBMPlexMono_24 = io.Fonts->AddFontFromMemoryTTF(IBMPlexMono_Bold_Data, sizeof(IBMPlexMono_Bold_Data), 24, &cfg);
	m_pIBMPlexMono_32 = io.Fonts->AddFontFromMemoryTTF(IBMPlexMono_Bold_Data, sizeof(IBMPlexMono_Bold_Data), 32, &cfg);

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(g_hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	return 1;
}

bool MyImGui::OnFrameStart()
{
	// Poll and handle messages (inputs, window resize, etc.)
	// See the WndProc() function below for our to dispatch events to the Win32 backend.
	MSG msg;
	while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			g_Alive = false;
	}

	// Handle window being minimized or screen locked
	if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
	{
		::Sleep(10);
		return 0;
	}
	g_SwapChainOccluded = false;

	// Handle window resize (we don't resize directly in the WM_SIZE handler)
	if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
	{
		CleanupRenderTarget();
		g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
		g_ResizeWidth = g_ResizeHeight = 0;
		CreateRenderTarget();
	}

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	return 1;
}

bool MyImGui::OnFrameEnd()
{
	// Rendering
	ImGui::Render();
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
	g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);

	return 1;
}

bool MyImGui::Close()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(g_hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);

	return 1;
}

bool MyImGui::CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
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

void MyImGui::CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void MyImGui::CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void MyImGui::CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI MyImGui::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
		g_ResizeHeight = (UINT)HIWORD(lParam);
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

bool MainMenu::Render()
{
	ImGui::PushFont(MyImGui::m_pIBMPlexMono_32);

	std::string MenuTitle = "CyNickal GTA";


	auto TextSize = ImGui::CalcTextSize(MenuTitle.data(), MenuTitle.data() + MenuTitle.size());
	const auto& Style = ImGui::GetStyle();

	ImGui::SetNextWindowSizeConstraints({ TextSize.x + ((Style.FramePadding.x + Style.WindowPadding.x) * 2.0f), 200.0f }, { 1000.0f,1000.0f });

	ImGuiWindowFlags wnd = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::Begin("Main Menu", &m_MainMenu, wnd);

	auto WindowWidth = ImGui::GetContentRegionAvail().x;

	unsigned int Offset = (WindowWidth - TextSize.x) / 2;
	Offset -= Style.FramePadding.x;

	auto CursorPos = ImGui::GetCursorPos();
	CursorPos.x += Offset;

	ImGui::SetCursorPos(CursorPos);

	ImGui::Text(MenuTitle.c_str());

	ImGui::PopFont();

	if (ImGui::CollapsingHeader("Inspectors"))
	{
		ImGui::Indent();

		ImGui::Checkbox("Weapon Inspector", &WeaponInspector::m_WeaponInspector);

		ImGui::Checkbox("Vehicle Inspector", &VehicleInspector::m_VehicleInspector);

		ImGui::Checkbox("Blip Inspector", &BlipInspector::m_BlipInspector);

		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader("Editors"))
	{
		ImGui::Indent();

		ImGui::Checkbox("Weapon Editor", &WeaponEditor::m_WeaponEditor);

		ImGui::Checkbox("Vehicle Editor", &VehicleEditor::m_VehicleEditor);

		ImGui::Unindent();
	}


	if (ImGui::CollapsingHeader("Self"))
	{
		ImGui::Indent();

		ImGui::Checkbox("God Mode", &GodMode::m_GodMode);

		ImGui::Checkbox("Vehicle God Mode", &VehicleGodMode::m_VehicleGodMode);

		ImGui::Checkbox("Infinite Ammo", &InfiniteAmmo::m_InfiniteAmmo);

		ImGui::Checkbox("Never Wanted", &NeverWanted::m_NeverWanted);

		ImGui::Checkbox("Refresh Health", &RefreshHealth::m_RefreshHealth);
		if (RefreshHealth::m_RefreshHealth)
		{
			ImGui::SameLine();
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::SliderFloat("##Heal Threshold", &RefreshHealth::m_HealthThreshold, 0.1f, 0.99f);
		}
		
		if (ImGui::Button("Teleport To Waypoint"))
		{
			Teleport::m_RequestedTeleport = true;
		}

		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader("Fuser"))
	{
		ImGui::Indent();

		ImGui::Checkbox("Toggle", &Fuser::m_Fuser);

		ImGui::SetNextItemWidth(120.0f);
		ImGui::InputInt("Monitor", &Fuser::m_MonitorIndex);

		ImGui::SetNextItemWidth(120.0f);
		ImGui::InputInt("Width", &Fuser::m_Width);

		ImGui::SetNextItemWidth(120.0f);
		ImGui::InputInt("Height	", &Fuser::m_Height);

		ImGui::Unindent();
	}

	ImGui::End();

	return 1;
}