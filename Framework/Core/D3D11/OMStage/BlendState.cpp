#include "Framework.h"
#include "BlendState.h"
#include "../DX11_Helper.h"

std::map<Factor, D3D11_BLEND> BlendState::blendFactors = {
	std::make_pair(Factor::ZERO, D3D11_BLEND_ZERO),
	std::make_pair(Factor::ONE, D3D11_BLEND_ONE),
	std::make_pair(Factor::SRC_COLOR, D3D11_BLEND_SRC_COLOR),
	std::make_pair(Factor::INV_SRC_COLOR, D3D11_BLEND_INV_SRC_COLOR),
	std::make_pair(Factor::SRC_ALPHA, D3D11_BLEND_SRC_ALPHA),
	std::make_pair(Factor::INV_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA),
	std::make_pair(Factor::DEST_ALPHA, D3D11_BLEND_DEST_ALPHA),
	std::make_pair(Factor::INV_DEST_ALPHA, D3D11_BLEND_INV_DEST_ALPHA),
	std::make_pair(Factor::DEST_COLOR, D3D11_BLEND_DEST_COLOR),
	std::make_pair(Factor::INV_DEST_COLOR, D3D11_BLEND_INV_DEST_COLOR),
};

std::map<Operation, D3D11_BLEND_OP> BlendState::blendOperations = {
	std::make_pair(Operation::ADD, D3D11_BLEND_OP_ADD),
	std::make_pair(Operation::SUBTRACT, D3D11_BLEND_OP_SUBTRACT),
	std::make_pair(Operation::MIN, D3D11_BLEND_OP_MIN),
	std::make_pair(Operation::MAX, D3D11_BLEND_OP_MAX),
};

std::map<ColorMask, D3D11_COLOR_WRITE_ENABLE> BlendState::blendColorMasks = {
	std::make_pair(ColorMask::RED, D3D11_COLOR_WRITE_ENABLE_RED),
	std::make_pair(ColorMask::GREEN, D3D11_COLOR_WRITE_ENABLE_GREEN),
	std::make_pair(ColorMask::BLUE, D3D11_COLOR_WRITE_ENABLE_BLUE),
	std::make_pair(ColorMask::ALPHA, D3D11_COLOR_WRITE_ENABLE_ALPHA),
	std::make_pair(ColorMask::ALL, D3D11_COLOR_WRITE_ENABLE_ALL),
};

BlendState::BlendState(Context * context) : state(nullptr), backState(nullptr)
{
	graphics = context->GetSubsystem<Graphics>();
}

BlendState::~BlendState()
{
	Clear();
}

//wireframe -> solid -> wireframe 이렇게 만들면 고속생성을 함. (첫 wireframe의 포인터를 두번째 wireframe에 줌, 이 때, 참조 카운트가 1 올라감(1, 1, 2))
const uint BlendState::Create(const bool & blendEnable, const Factor & srcBlend, const Factor & destBlend, const Operation & blendOp, const Factor & srcBlendAlpha, const Factor & destBlendAlpha, const Operation & blendOpAlpha, const ColorMask & renderTargetWriteMask)
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = blendEnable;
	desc.RenderTarget[0].SrcBlend = blendFactors[srcBlend];
	desc.RenderTarget[0].DestBlend = blendFactors[destBlend];
	desc.RenderTarget[0].BlendOp = blendOperations[blendOp];
	desc.RenderTarget[0].SrcBlendAlpha = blendFactors[srcBlendAlpha];
	desc.RenderTarget[0].DestBlendAlpha = blendFactors[destBlendAlpha];
	desc.RenderTarget[0].BlendOpAlpha = blendOperations[blendOpAlpha];
	desc.RenderTarget[0].RenderTargetWriteMask = blendColorMasks[renderTargetWriteMask];

	auto hr = graphics->GetDevice()->CreateBlendState(&desc, &state);
	assert(SUCCEEDED(hr));

	return DX11_Helper::CreateBitMask(blendEnable, srcBlend, destBlend, blendOp, srcBlendAlpha, destBlendAlpha, blendOpAlpha, renderTargetWriteMask);
}

void BlendState::Clear()
{
	SAFE_RELEASE(backState);
	SAFE_RELEASE(state);
}

void BlendState::BindPipeline()
{
	graphics->GetDeviceContext()->OMGetBlendState(&backState, nullptr, nullptr);
	graphics->GetDeviceContext()->OMSetBlendState(state, nullptr, 0xffffffff);
}

void BlendState::UnbindPipeline()
{
	graphics->GetDeviceContext()->OMSetBlendState(backState, nullptr, 0xffffffff);
	SAFE_RELEASE(backState);
}
