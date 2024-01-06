#include <cstdio>
#include <Windows.h>
#include <d3d11.h>
#include <dwmapi.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#include "Valve/Precompiled.h"

#pragma comment(lib,"d3d11.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
		return 0L;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


INT APIENTRY WinMain(HINSTANCE hInstance , HINSTANCE prevInstance,PSTR pStr ,int cmdShow) {
	WNDCLASSEXW wc{};
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProcedure;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"External Overlay Class";

	RegisterClassExW(&wc);

	const HWND window = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
		wc.lpszClassName,
		L"D3D11 Overlay",
		WS_POPUP,
		0,
		0,
		GetSystemMetrics(SM_CXFULLSCREEN),
		GetSystemMetrics(SM_CYFULLSCREEN),
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

	{
		RECT* client_area = new RECT;
		GetClientRect(window, client_area);

		RECT* window_area = new RECT;
		GetWindowRect(window, window_area);

		POINT* diff = new POINT;
		ClientToScreen(window, diff);

		const MARGINS* margins = new MARGINS{
			window_area->left + (diff->x - window_area->left),
			window_area->top + (diff->y - window_area->top),
			client_area->right,
			client_area->bottom
		};

		DwmExtendFrameIntoClientArea(window, margins);

		delete[] client_area;
		delete[] window_area;
		delete[] diff;
	}

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.RefreshRate.Numerator = 150U;
	sd.BufferDesc.RefreshRate.Denominator = 1U;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1U;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2U;
	sd.OutputWindow = window;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	constexpr D3D_FEATURE_LEVEL levels[2]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};


	ID3D11Device* device{ nullptr };
	ID3D11DeviceContext* deviceContext{ nullptr };
	IDXGISwapChain* swapChain{ nullptr };
	ID3D11RenderTargetView* renderTargetView{ nullptr };
	D3D_FEATURE_LEVEL level;

	D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0U,
		levels,
		2U,
		D3D11_SDK_VERSION,
		&sd,
		&swapChain,
		&device,
		&level,
		&deviceContext);

	ID3D11Texture2D* backBuffer;
	swapChain->GetBuffer(0U, IID_PPV_ARGS(&backBuffer));

	if (backBuffer) {
		device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
		backBuffer->Release();
	}
	else {
		return 1;
	}

	ShowWindow(window, cmdShow);
	UpdateWindow(window);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, deviceContext);
	
	bool running = true;
	
	while (running) {
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT || interfaces::mem.GetProcessId() == 0) {
				running = false;
			}
			if (!running)
				break;

		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();
		interfaces::vm = interfaces::mem.Read<ViewMatrix_t>(interfaces::client + offsets::client::dwViewMatrix);
		LocalPlayer lp = LocalPlayer();

		for (int i = 0; i <= 32; i++) {
			CPlayer player = CPlayer(i);

			if (player.m_iHealth() <= 0)
				continue;

			if (player.m_iTeamNum() == lp.m_iTeamNum())
				continue;

			if (!player.Valid())
				continue;

			if (player.Entity() == lp.Entity())
				continue;

			if (!interfaces::mem.IsMainWindow())
				continue;

			Vector_t FeetPos = player.m_VecOrigin().W2S(interfaces::vm);
			Vector_t HeadPos = (player.Bone_Origin(6) + Vector_t(0.f, 0.f, 7.f)).W2S(interfaces::vm);

			if (FeetPos.z <= 0.001f)
				continue;

			float height = HeadPos.y - FeetPos.y;
			float width =  height * 0.25f;

			ImGui::GetBackgroundDrawList()->AddRect(
				ImVec2(FeetPos.x - (int)width, FeetPos.y),
				ImVec2(HeadPos.x + (int)width, HeadPos.y ),
				ImColor(255, 255, 255)
			);
			
		}
		
		//Render

		ImGui::Render();

		constexpr float color[4] = { 0.f,0.f,0.f,0.f };
		deviceContext->OMSetRenderTargets(1U, &renderTargetView, nullptr);
		deviceContext->ClearRenderTargetView(renderTargetView, color);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		swapChain->Present(1U, 0U);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (device)
		device->Release();

	if (deviceContext)
		deviceContext->Release();

	if (swapChain)
		swapChain->Release();

	if (renderTargetView)
		renderTargetView->Release();

	DestroyWindow(window);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
	return 0;
}