#pragma once
#include<d3d12.h>
#include<DirectXMath.h>
#include<wrl/client.h>

#include<string_view>
#include<vector>
#include<cstdint>
#include<memory>

#include"../directx/PiplineStateObject.h"
#include"../directx/CommandList.h"
#include"../window/Window_Base.h"
#include"../MoveProhibited/Nomove.h"

class TextRender final : public Nomove
{
public:
	TextRender() = default;
	~TextRender() = default;

	/// <summary> 
	/// 文字描画に必要なリソースを作成する
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] bool Initialize()noexcept;

	/// <summary>
	/// 文字を描画する
	/// </summary>
	/// <param name="commandList"></param>
	/// <param name="frameIndex"></param>
	/// <param name="text"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="fontSize"></param>
	/// <param name="windowInstance"></param>
	/// <param name="color"></param>
	void TextDraw(const CommandList& commandList, 
		          const UINT frameIndex, 
		          const std::string_view text, 
		          const float x, const float y, 
		          const float fontSize, 
		          const std::unique_ptr<Window_Base>& windowInstance, 
		          const DirectX::XMFLOAT4 color) noexcept;

private:
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	static constexpr UINT FrameCount = 2;
	static constexpr UINT MaxCharacters = 256;
	static constexpr UINT VerticesPerCharacter = 5 * 7 * 6;

	static constexpr UINT MaxVertices = MaxCharacters * VerticesPerCharacter;

	/// <summary>
	/// 1文字の5×7ドットデータを取得する
	/// </summary>
	/// <param name="character"></param>
	/// <returns></returns>
	[[nodiscard]] std::vector<uint8_t> GetGlyph(char character) const noexcept;

	/// <summary>
	/// 画面上の座標を頂点に変換して追加する
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	/// <param name="screenWidth"></param>
	/// <param name="screenHeight"></param>
	/// <param name="color"></param>
	void AddVertex(
		const float x,
		const float y,
		const float z,
		const std::unique_ptr<Window_Base>& windowInstance,
		const DirectX::XMFLOAT4 color
	);

	/// <summary>
	/// 四角形を2つの三角形として追加する
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	/// <param name="windowInstance"></param>
	/// <param name="color"></param>
	void AddQuad(
		const float x,
		const float y,
		const float z,
		const std::unique_ptr<Window_Base>& windowInstance,
		const DirectX::XMFLOAT4 color
	);

	/// <summary>
	/// 文字描画用のルートシグネチャを作成する
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] bool CreateRootSignature() noexcept;

	/// <summary>
	/// 文字描画用のパイプラインを作成する
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] bool CreatePipelineState() noexcept;

	/// <summary>
	/// 頂点バッファを作成する
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] bool CreateVertexBuffers() noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;

	/// <summary>
	/// 2フレーム分の頂点バッファを保持する
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffers_[FrameCount];

	/// <summary>
	/// GPUへ転送するデータを書き込むためのポインタ
	/// </summary>
	uint8_t* mappedData_[FrameCount]{};

	D3D12_VERTEX_BUFFER_VIEW vertexBufferViews_[FrameCount]{};

	/// <summary>
	/// 今回のフレームで描画する頂点データ
	/// </summary>
	Vertex vertices_[MaxVertices]{};

	UINT vertexCount_ = 0;
};