#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "Vector.h"
#include <string>
#include <Sprite.h>

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
	void PrintVector(std::string str, Vector2 vec);
	void PrintVector(std::string str, Vector3 vec);
	void SliderVector(std::string str, Vector2& vec);
	void SliderVector(std::string str, Vector3& vec);
	void ColorEdit(Color& color);
};