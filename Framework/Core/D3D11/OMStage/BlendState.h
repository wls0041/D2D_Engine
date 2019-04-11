#pragma once
#include "../IPipeline.h"

enum class Factor : uint
{
	ZERO = 1U << 2,
	ONE = 1U << 3,
	SRC_COLOR = 1U << 4,
	INV_SRC_COLOR = 1U << 5,
	SRC_ALPHA = 1U << 6,
	INV_SRC_ALPHA = 1U << 7,
	DEST_ALPHA = 1U << 8,
	INV_DEST_ALPHA = 1U << 9,
	DEST_COLOR = 1U << 10,
	INV_DEST_COLOR = 1U << 11,
};

enum class Operation : uint
{
	ADD = 1U << 12,
	SUBTRACT = 1U << 13,
	MIN = 1U << 14,
	MAX = 1U << 15,
};

enum class ColorMask : uint
{
	RED = 1U << 16,
	GREEN = 1U << 17,
	BLUE = 1U << 18,
	ALPHA = 1U << 19,
	ALL = RED | GREEN | BLUE | ALPHA,
};

enum BlendMode : uint
{
	Blend_None = uint(Factor::ONE) | uint(Factor::ZERO) | uint(Operation::ADD) | uint(Factor::ONE) | uint(Factor::ZERO) | uint(Operation::ADD) | uint(ColorMask::ALL) | uint(0),
	Blend_Add = uint(Factor::ONE) | uint(Factor::ONE) | uint(Operation::ADD) | uint(Factor::ONE) | uint(Factor::ONE) | uint(Operation::ADD) | uint(ColorMask::ALL) | uint(1),
	Blend_Multiply = uint(Factor::DEST_COLOR) | uint(Factor::ZERO) | uint(Operation::ADD) | uint(Factor::ONE) | uint(Factor::ONE) | uint(Operation::ADD) | uint(ColorMask::ALL) | uint(1),
	Blend_Alpha = uint(Factor::SRC_ALPHA) | uint(Factor::INV_SRC_ALPHA) | uint(Operation::ADD) | uint(Factor::ONE) | uint(Factor::ONE) | uint(Operation::ADD) | uint(ColorMask::ALL) | uint(1),
	Blend_Screen = uint(Factor::ONE) | uint(Factor::INV_SRC_COLOR) | uint(Operation::ADD) | uint(Factor::ONE) | uint(Factor::ONE) | uint(Operation::ADD) | uint(ColorMask::ALL) | uint(1),
};

class BlendState final : public IPipeline
{
public:
	BlendState(class Context* context);
	~BlendState();

	BlendState(const BlendState&) = delete;
	BlendState& operator=(const BlendState&) = delete;

	const uint Create
	(
		const bool& blendEnable,
		const Factor& srcBlend,
		const Factor& destBlend,
		const Operation& blendOp,
		const Factor& srcBlendAlpha,
		const Factor& destBlendAlpha,
		const Operation& blendOpAlpha,
		const ColorMask& renderTargetWriteMask = ColorMask::ALL
	);
	void Clear();
	void BindPipeline();
	void UnbindPipeline();

private:
	class Graphics* graphics;

	ID3D11BlendState* state;
	ID3D11BlendState* backState;

private:
	static std::map<Factor, D3D11_BLEND> blendFactors;
	static std::map<Operation, D3D11_BLEND_OP> blendOperations;
	static std::map<ColorMask, D3D11_COLOR_WRITE_ENABLE> blendColorMasks;
};