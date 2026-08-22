#pragma

#include<memory>

#include"../../MoveProhibited/Nomove.h"
#include"../../window/Window_Base.h"

#include"../../directx/Device.h"
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

#include"../../draw_resource/QuadPolygon.h"

#include"../../object/Camera.h"
#include"../../object/Object.h"

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
	Device deviceInstance_{};						 //デバイスインスタンス
	CommandQueue commandQueueInstance_{};			 //コマンドキューインスタンス
	SwapChain swapChainInstance_{};					 //スワップチェインインスタンス
	DescriptorHeap descriptorHeapInstance_{};		 //ディスクリプタヒープインスタンス
	RenderTarget renderTargetInstance_{};			 //レンダーターゲットインスタンス
	CommandAllocator commandAllocatorInstance_[2]{}; //コマンドアロケータインスタンス 
	CommandList commandListInstance_{};              //コマンドリストインスタンス

	Fence fenceInstance_{};       //フェンスインスタンス
	UINT64 frameFenceValue_[2]{}; //現在のフレームのフェンス値
	UINT64 nextFenceValue_ = 1;	  //次のフレームのフェンス値

	RootSignature      rootSignatureInstance_{};                 /// ルートシグネチャインスタンス
	Shader             shaderInstance_{};                        /// シェーダーインスタンス
	PiplineStateObject piplineStateObjectInstance_{};            /// パイプラインステートオブジェクトインスタンス
	DescriptorHeap     constantBufferDescriptorHeapInstance_{};  /// コンスタントバッファ用ディスクリプタヒープインスタンス

	//
	TrianglePolygon trianglePolygonInstance_{};                /// 三角形ポリゴンインスタンス
	Object          triangleObjectInstance_{};                 /// 三角形オブジェクトインスタンス
	ConstantBuffer  trianglePolygonConstantBufferInstance_{};  /// 三角形ポリゴン用コンスタントバッファインスタンス

	//
	Camera         cameraInstance_{};                /// カメラインスタンス
	ConstantBuffer cameraConstantBufferInstance_{};  /// カメラ用コンスタントバッファインスタンス
};
	