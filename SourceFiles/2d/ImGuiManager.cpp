#include "ImGuiManager.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#include "SpriteCommon.h"
using namespace ImGui;

ID3D12DescriptorHeap* ImGuiManager::srvHeap_;

void ImGuiManager::Initialize()
{
	// インスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// ImGuiのコンテキストを生成
	CreateContext();
	// ImGuiのスタイルを設定
	StyleColorsDark();

	ImGui_ImplWin32_Init(WindowsAPI::GetInstance()->GetHwnd());

	SpriteCommon* spCommon = SpriteCommon::GetInstance();
	srvHeap_ = spCommon->GetDescriptorHeap();

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap_->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap_->GetGPUDescriptorHandleForHeapStart();
	srvHandle.ptr += spCommon->GetIncrementSize();
	srvGpuHandle.ptr += spCommon->GetIncrementSize();

	ImGui_ImplDX12_Init(dxCommon->GetDevice(),
		static_cast<int>(dxCommon->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_,
		srvHandle, srvGpuHandle);

	spCommon->IncrementTextureIndex();

	ImGuiIO& io = GetIO();
	// 標準フォントを追加する
	io.Fonts->AddFontDefault();
}

void ImGuiManager::Begin()
{
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();
}

void ImGuiManager::End() { Render(); }

void ImGuiManager::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_ };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	ImGui_ImplDX12_RenderDrawData(GetDrawData(), cmdList);
}

void ImGuiManager::Finalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	DestroyContext();
}

void ImGuiManager::PrintVector(std::string& str, const Vector2& vec)
{
	str += " = (%.3f, %.3f)";
	Text(str.c_str(), vec.x, vec.y);
}

void ImGuiManager::PrintVector(std::string& str, const Vector3& vec)
{
	str += " = (%.3f, %.3f, %.3f)";
	Text(str.c_str(), vec.x, vec.y, vec.z);
}

void ImGuiManager::SliderVector(std::string& str, Vector2& vec)
{
	float num[2] = { vec.x,vec.y };
	SliderFloat2(str.c_str(), num, 0, WindowsAPI::WIN_SIZE.x);
	vec = { num[0],num[1] };
}

void ImGuiManager::SliderVector(std::string& str, Vector3& vec)
{
	float num[3] = { vec.x,vec.y,vec.z };
	SliderFloat3(str.c_str(), num, 0, WindowsAPI::WIN_SIZE.x);
	vec = { num[0],num[1],num[2] };
}

void ImGuiManager::ColorEdit(Color& color)
{
	float c[4] = { color.r.GetVal(),color.g.GetVal(),color.b.GetVal(),color.a.GetVal() };
	ColorEdit4("Color", c);
	color = { c[0],c[1],c[2],c[3] };
}