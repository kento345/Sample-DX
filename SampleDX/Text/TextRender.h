#pragma once
#include<d3d12.h>
#include<DirectXMath.h>
#include<string_view>
#include<unordered_map>
#include<wrl/client.h>
#include<memory>

#include"../directx/PiplineStateObject.h"
#include"../window/Window_Base.h"
#include"../MoveProhibited/Nomove.h"

class TextRender final : public Nomove
{
public:
	TextRender() = default;
	~TextRender() = default;

	[[nodiscard]] bool Initialize(const PiplineStateObject& piplineInstance, const std::unique_ptr<Window_Base>& window) noexcept;

	void Draw(const std::wstring_view text, float x, float y,float fontSize, const DirectX::XMFLOAT4& color)noexcept;

	void Finalize();

private	:

	void createTextVertices(const std::wstring_view text, float x, float y, float fontSize, const DirectX::XMFLOAT4& color)noexcept;


};

