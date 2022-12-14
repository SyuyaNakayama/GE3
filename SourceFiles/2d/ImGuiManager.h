#pragma once
#include <wrl.h>
#include <d3d12.h>

class ImGuiManager final
{
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
	ImGuiManager() = default;

public:
	static ImGuiManager* GetInstance();
	ImGuiManager(const ImGuiManager& obj) = delete;

	void Initialize();
	void Begin();
	void End();
	void Draw();
	void Finalize();
};