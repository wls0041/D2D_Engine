#pragma once
#include "../IPipeline.h"

enum class Factor : uint {
	ZERO				= 1U << 0,
	ONE					= 1U << 1,
	SRC_COLOR			= 1U << 2,
	INV_SRC_COLOR		= 1U << 3,
	SRC_ALPHA			= 1U << 4,
	INV_SRC_ALPHA		= 1U << 5,
	DEST_ALPHA			= 1U << 6,
	INV_DEST_ALPHA		= 1U << 7,
	DEST_COLOR			= 1U << 8,
	INV_DEST_COLOR		= 1U << 9,
};

enum class Operation : uint {
	ADD			= 1U << 10,
	SUBTRACT	= 1U << 11,
	MIN			= 1U << 12,
	MAX			= 1U << 13,
};

enum class ColorMask : uint {
	RED		= 1U << 14,
	GREEN	= 1U << 15,
	BLUE	= 1U << 16,
	ALPHA	= 1U << 17,
	ALL		= RED | GREEN | BLUE | ALPHA,
};
class BlendState final : public IPipeline
{
public:
	BlendState(class Context *context);
	~BlendState();

	BlendState(const BlendState&) = delete;
	BlendState &operator=(const BlendState&) = delete;

	const uint Create(const bool &blendEnable, const Factor &srcBlend, const Factor &destBlend, const Operation &blendOp, 
					  const Factor &srcBlendAlpha, const Factor &destBlendAlpha, const Operation &blendOpAlpha, const ColorMask &renderTargetWriteMask = ColorMask::All);
	void Clear();
	void BindPipeline();
	void UnbindPipeline();
	
private:
	class Graphics *graphics;

	ID3D11BlendState *state;
	ID3D11BlendState *backState;

private:
	static std::map<Factor, D3D11_BLEND> blendFactors;
	static std::map<Operation, D3D11_BLEND_OP> blendOperations;
	static std::map<ColorMask, D3D11_COLOR_WRITE_ENABLE> blendColorMasks;
};