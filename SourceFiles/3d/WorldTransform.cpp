#include "WorldTransform.h"
#include "Functions.h"

ViewProjection* WorldTransform::viewProjection_ = nullptr;
std::unique_ptr<LightGroup> WorldTransform::lightGroup;

void WorldTransform::Initialize()
{
	CreateBuffer(constBuffer.GetAddressOf(), &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}

void WorldTransform::Update()
{
	assert(viewProjection_);

	Matrix4 matScale = Matrix4::Scale(scale);
	Matrix4 matRot = Matrix4::Rotate(rotation);
	Matrix4 matTrans = Matrix4::Translate(translation);
	matWorld = matScale * matRot * matTrans;
	if (parent) { matWorld *= parent->matWorld; }

	constMap->world = matWorld;
	constMap->viewproj = viewProjection_->GetViewProjectionMatrix();
	constMap->cameraPos = viewProjection_->eye;
}