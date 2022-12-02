#pragma once
#include "Sprite.h"
#include <memory>
#include "Framework.h"

class MyGame : public Framework
{
private:
	// �Q�[�����[�v�Ŏg���ϐ��̒�`
	std::vector<Sprite*> sprites{};

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};