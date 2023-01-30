#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <string>
#include "Sprite.h"
#include "Quaternion.h"
#include "WindowsAPI.h"

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
	static void PrintVector(std::string str, const Vector2& vec);
	static void PrintVector(std::string str, const Vector3& vec);
	static void PrintQuaternion(std::string str, const Quaternion& q);
	static void SliderVector(std::string str, Vector2& vec, float minVal, float maxVal);
	static void SliderVector(std::string str, Vector3& vec, float minVal, float maxVal);
	static void ColorEdit(Color& color);
};