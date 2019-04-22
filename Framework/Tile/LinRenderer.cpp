#include "Framework.h"
#include "LinRenderer.h"

LinRenderer::LinRenderer(Context * context) : context(context)
{
	vertexBuffer = new VertexBuffer(context);
	vertexBuffer->Create(vertices, D3D11_USAGE_DYNAMIC); //������ ��� �ٲ��ֱ� ����. default�� �ϰ� subresource�� ������ �վ��� �� ������ dynamic�� map, unmap���� �о� �ִ°� �� ����
}

LinRenderer::~LinRenderer()
{
}
