#pragma once

class LinRenderer final
{
public:
	LinRenderer(class Context *context);
	~LinRenderer();

private:
	class Context *context;
	class Shader *shader;
	class VertexBuffer *vertexBuffer;

	std::vector<VertexColor> vertices;
	uint vertexCount;
};