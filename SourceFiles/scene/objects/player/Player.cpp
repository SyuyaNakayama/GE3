#include "Player.h"
#include "ImGuiManager.h"
#include "Functions.h"
#include "SharePtr.h"
#include <imgui.h>

using namespace std;

void Player::Initialize()
{
	worldTransform.Initialize();
	model = Model::Create("player");
	WorldTransform::GetViewProjection()->target =
		worldTransform.translation + Vector3(0, 0, 20);
	SetCollisionAttribute(CollisionAttribute::Player);
	SetCollisionMask(CollisionMask::Player);
	SharePtr::SetPlayer(this);
	shotSE = Audio::Create(L"shotSE.mp3");
}

void Player::Move()
{
	float moveSpd = 1.0f;

	Vector3 moveVel =
	{
		input->Move(DIK_RIGHT,DIK_LEFT,moveSpd),
		input->Move(DIK_UP,DIK_DOWN,moveSpd)
	};

	worldTransform.translation += moveVel;
	Clamp(worldTransform.translation.x, -LIMIT_POS.x, LIMIT_POS.x);
	Clamp(worldTransform.translation.y, -2.0f, LIMIT_POS.y);
}

void Player::Shot()
{
	bullets.remove_if([](unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	if (input->IsTrigger(DIK_SPACE))
	{
		unique_ptr<PlayerBullet> newBullet = make_unique<PlayerBullet>();
		newBullet->Initialize(worldTransform.translation, Vector3(0, 0, 1.5f));
		bullets.push_back(move(newBullet));
		shotSE->Play();
	}

	for (const unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->Update();
	}

	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	//ImGui::Text("bulletNum = %d", bullets.size());
}

void Player::Update()
{
	Move();
	worldTransform.Update();
	Shot();
}

void Player::Draw()
{
	model->Draw(worldTransform);
	for (const unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->Draw();
	}
}

void Player::OnCollision(Collider* collider)
{
	hp--;
	ImGui::Text("Hit!");
}