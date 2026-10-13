#pragma
#include"../../MoveProhibited/Nomove.h"
#include"../../window/Window_Base.h"

#include"../../directx/DXGI.h"
#include"../../directx/CommandAllocator.h"
#include"../../directx/CommandList.h"
#include"../../directx/CommandQueue.h"
#include"../../directx/SwapChain.h"
#include"../../directx/DescriptorHeap.h"
#include"../../directx/RenderTarget.h"
#include"../../directx/Fence.h"
#include"../../directx/RootSignature.h"
#include"../../directx/Shader.h"
#include"../../directx/PiplineStateObject.h"
#include"../../directx/ConstantBuffer.h"
#include"../../directx/DepthBuffer.h"

#include"../../draw_resource/TrianglePolygon.h"
#include"../../draw_resource/QuadPolygon.h"
#include"../../draw_resource/ShapeContainer.h"

#include"../../object/Player.h"
#include"../../object/Enemy.h"
#include"../../object/Camera.h"

#include<memory>
#include<cassert>

struct HWND__;

using HWND = HWND__*;

struct HINSTANCE__;

using HINSTANCE = HINSTANCE__*;

class Application : public Nomove
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Application() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Application() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>初期化できたらtrueを返す</returns>
	[[nodiscard]] bool initialize(HINSTANCE hInstance);

	/// <summary>
	/// ループ処理
	/// </summary>
	[[nodiscard]] void loop()noexcept;

private:
	std::unique_ptr<Window_Base> windowInstance_;    //ウィンドウインスタンス
	CommandQueue commandQueueInstance_{};			 //コマンドキューインスタンス
	SwapChain swapChainInstance_{};					 //スワップチェインインスタンス
	RenderTarget renderTargetInstance_{};			 //レンダーターゲットインスタンス
	DepthBuffer depthBufferInstance_{};              //デプスバッファインスタンス
	CommandAllocator commandAllocatorInstance_[2]{}; //コマンドアロケータインスタンス 
	CommandList commandListInstance_{};              //コマンドリストインスタンス

	Fence fenceInstance_{};       //フェンスインスタンス
	UINT64 frameFenceValue_[2]{}; //現在のフレームのフェンス値
	UINT64 nextFenceValue_ = 1;	  //次のフレームのフェンス値

	RootSignature      rootSignatureInstance_{};                 /// ルートシグネチャインスタンス
	Shader             shaderInstance_{};                        /// シェーダーインスタンス
	PiplineStateObject piplineStateObjectInstance_{};            /// パイプラインステートオブジェクトインスタンス
	
	//カメラ
	std::unique_ptr<game::Camera> camera_{};
};
	