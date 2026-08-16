#include "Camera.h"
#include<cmath>

namespace {
	//定数
	constexpr float eyeMoveSpeed_ = 0.06f;     //カメラ移動速度
	constexpr float destTargetToView_ = -5.0f; //注視点カメラまでの距離
}


void Camera::initialize() noexcept
{
	//カメラの位置を設定
	position_ = DirectX::XMFLOAT3(0.0f, 0.0f, destTargetToView_);
	//カメラの注視点を設定
	target_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	//カメラの上方向を設定
	up_ = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

	//プロジェクション行列の設定
	projection_ - DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XM_PIDIV4, //視野角45度
		1280.0f / 720.0f,	//アスペクト比
		0.1f,				//ニアクリップ
		100.0f				//ファークリップ
	);
}

void Camera::update() noexcept
{
	static float angle = 0.0f;
	angle += eyeMoveSpeed_;
	position_.x = destTargetToView_ * std::sinf(angle);
	position_.z = destTargetToView_ * std::cosf(angle);

	//ビュー行列計算
	view_ = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&position_),
		DirectX::XMLoadFloat3(&target_),
		DirectX::XMLoadFloat3(&up_));
}

DirectX::XMMATRIX XM_CALLCONV Camera::viewMatrix() const noexcept
{
	return view_;
}

DirectX::XMMATRIX XM_CALLCONV Camera::projection() const noexcept
{
	return projection_;
}
