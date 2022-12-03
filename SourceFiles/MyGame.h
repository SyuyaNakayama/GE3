#pragma once
#include <memory>
#include "Framework.h"

class MyGame : public Framework
{
private:

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};