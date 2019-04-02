#include "Framework.h"
#include "Light.h"

Light::Light(Context * context, GameObject * object, Transform * transform) : IComponent(context, object, transform), color(1.0f), minDistance(0.0f), maxDistance(0.0f), distanceFactor(0.0f), bTwinkle(false)
{
}

Light::~Light()
{
}

void Light::OnInitialize()
{
}

void Light::OnStart()
{
}

void Light::OnUpdate()
{
}

void Light::OnStop()
{
}

void Light::OnDestroy()
{
}
