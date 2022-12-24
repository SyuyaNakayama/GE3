#include "Player.h"
#include "ImGuiManager.h"
#include <imgui.h>

using namespace std;

void Player::Initialize()
{
	worldTransform.Initialize();
	SetCollisionAttribute(CollisionAttribute::Player);
	model = Model::Create("player");
	worldTransform.translation = { -50,0,-50 };
	WorldTransform::GetViewProjection()->target =
		worldTransform.translation + Vector3(0, 0, 20);
}

void Player::Move()
{
	float moveSpd = 1.0f;

	Vector3 moveVel =
	{
		input->Move(DIK_RIGHT,DIK_LEFT,moveSpd),
		input->Move(DIK_UP,DIK_DOWN,moveSpd),moveSpd / 12.0f
	};

	worldTransform.translation += moveVel;
	ViewProjection* v = WorldTransform::GetViewProjection();
	v->CameraMove(Vector3(0, 0, moveVel.z));
}

void Player::Shot()
{
	bullets.remove_if([](unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	if (input->IsTrigger(DIK_SPACE))
	{
		unique_ptr<PlayerBullet> newBullet = make_unique<PlayerBullet>();
		newBullet->Initialize(worldTransform.translation, Vector3(0, 0, 1));
		bullets.push_back(move(newBullet));
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