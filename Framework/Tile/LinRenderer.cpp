#include "Framework.h"
#include "LinRenderer.h"

LinRenderer::LinRenderer(Context * context) : context(context)
{
	vertexBuffer = new VertexBuffer(context);
	vertexBuffer->Create(vertices, D3D11_USAGE_DYNAMIC); //정점을 계속 바꿔주기 위해. default로 하고 subresource로 강제로 뚫어줄 수 있지만 dynamic의 map, unmap으로 밀어 넣는게 더 빠름
}

LinRenderer::~LinRenderer()
{
}
