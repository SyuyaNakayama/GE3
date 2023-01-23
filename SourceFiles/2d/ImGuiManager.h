#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "Vector.h"
#include <string>
#include <Sprite.h>

class ImGuiManager final
{
private:
	static ID3D12DescriptorHeap* srvHeap_;
	ImGuiManager() = delete;
	~ImGuiManager() = delete;

public:
	static void Initialize();
	static void Begin();
	static void End();
	static void Draw();
	static void Finalize();
	static void PrintVector(std::string& str, const Vector2& vec);
	static void PrintVector(std::string& str, const Vector3& vec);
	static void SliderVector(std::string& str, Vector2& vec);
	static void SliderVector(std::string& str, Vector3& vec);
	static void ColorEdit(Color& color);
};