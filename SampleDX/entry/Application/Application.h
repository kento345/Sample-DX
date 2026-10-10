#pragma

#include<memory>

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

#include"../../draw_resource/TrianglePolygon.h"
#include"../../object/Player.h"

#include"../../draw_resource/QuadPolygon.h"
#include"../../object/Enemy.h"

#include"../../object/Camera.h"


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
	DXGI dxgiInstance_{};							 //DXGIインスタンス
	CommandQueue commandQueueInstance_{};			 //コマンドキューインスタンス
	SwapChain swapChainInstance_{};					 //スワップチェインインスタンス
	RenderTarget renderTargetInstance_{};			 //レンダーターゲットインスタンス
	CommandAllocator commandAllocatorInstance_[2]{}; //コマンドアロケータインスタンス 
	CommandList commandListInstance_{};              //コマンドリストインスタンス

	Fence fenceInstance_{};       //フェンスインスタンス
	UINT64 frameFenceValue_[2]{}; //現在のフレームのフェンス値
	UINT64 nextFenceValue_ = 1;	  //次のフレームのフェンス値

	RootSignature      rootSignatureInstance_{};                 /// ルートシグネチャインスタンス
	Shader             shaderInstance_{};                        /// シェーダーインスタンス
	PiplineStateObject piplineStateObjectInstance_{};            /// パイプラインステートオブジェクトインスタンス

	//三角形ポリゴン
	TrianglePolygon trianglePolygonInstance_{};                /// 三角形ポリゴンインスタンス
	Player          triangleObjectInstance_{};                 /// 三角形オブジェクトインスタンス
	ConstantBuffer  trianglePolygonConstantBufferInstance_{};  /// 三角形ポリゴン用コンスタントバッファインスタンス

	//四角形ポリゴン
	QuadPolygon    quadPolygonInstance_{};				//四角形ポリゴンインスタンス
	Enemy          quadObjectInstance_{};               //四角形オブジェクトインスタンス
	ConstantBuffer quadPolygonConstanBufferInstance_{}; //四角形ポリゴン用コンスタントバッファインスタンス

	//カメラ
	std::unique_ptr<game::Camera> camera_{};
};
	