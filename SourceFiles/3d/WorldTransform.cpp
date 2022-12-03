#include "WorldTransform.h"
#include "Functions.h"

void WorldTransform::Initialize()
{
	BufferMapping(constBuffer.GetAddressOf(), &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}

void WorldTransform::Update()
{
	Matrix4 matScale = Scale(scale);
	Matrix4 matRot = Rotate(rotation);
	Matrix4 matTrans = Translate(translation);
	matWorld = matScale * matRot * matTrans;
	if (parent != nullptr) { matWorld *= parent->matWorld; }

	constMap->mat = matWorld;
}