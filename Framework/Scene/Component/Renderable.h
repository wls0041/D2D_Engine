#pragma once
#include "IComponent.h"

//ȭ�鿡 �׸� ������Ʈ�� �� class�� ������ �־�� ��
class Renderable final : public IComponent
{
public:
	Renderable(class Context* context, class GameObject *object, class Transform *transform);
	~Renderable();

	Renderable(const Renderable &rhs) = delete;
	Renderable &operator=(const Renderable &rhs) = delete;

	void OnInitialize() override;
	void OnStart() override; //�����ϴ� ����
	void OnUpdate() override;
	void OnStop() override; //���� ��
	void OnDestroy() override;

	class Material *GetMaterial() const { return material; }
	class Mesh *GetMesh() const { return mesh; }
	const BoundBox GetBoundBox();

	void SetMaterial(class Material *material) { this->material = material; }
	void SetMaterial(const std::string &materialName);
	void SetStandardMaterial();

	void SetMesh(class Mesh *mesh) { this->mesh = mesh; }
	void SetMesh(const std::string &meshName);
	void SetStandardMesh(const MeshType &type);

private:
	class ResourceManager *resourceMgr;
	class Material *material;
	class Mesh *mesh;
	BoundBox boundBox;
};