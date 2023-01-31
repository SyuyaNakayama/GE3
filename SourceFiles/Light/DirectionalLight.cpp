#include "DirectionalLight.h"

void DirectionalLight::SetLightDir(const Vector3& lightdir)
{
	this->lightdir = Normalize(lightdir);
}

void DirectionalLight::SetLightColor(const ColorRGB& lightcolor)
{
	this->lightcolor = lightcolor;
}