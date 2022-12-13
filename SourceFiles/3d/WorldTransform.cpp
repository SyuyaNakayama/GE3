#include "WorldTransform.h"
#include "Functions.h"
ViewProjection* WorldTransform::viewProjection_ = nullptr;

void WorldTransform::Initialize()
{
	BufferMapping(constBuffer.GetAddressOf(), &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}

void WorldTransform::Update()
{
	assert(viewProjection_);
	
	Matrix4 matScale = Matrix4::Scale(scale);
	Matrix4 matRot = Matrix4::Rotate(rotation);
	Matrix4 matTrans = Matrix4::Translate(translation);
	matWorld = matScale * matRot * matTrans;
	if (parent != nullptr) { matWorld *= parent->matWorld; }

	constMap->mat = matWorld* viewProjection_->GetViewProjectionMatrix();
}