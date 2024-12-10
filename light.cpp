#include "light.h"
Light::Light() {
}
void Light::SetAmbientColor(float r, float g, float b, float a) {
	float ambientColor[] = { r,g,b,a };
	glLightfv(mLightIdentifier, GL_AMBIENT, ambientColor);
}
void Light::SetDiffuseColor(float r, float g, float b, float a) {
	float diffuseColor[] = { r,g,b,a };
	glLightfv(mLightIdentifier, GL_DIFFUSE, diffuseColor);
}
void Light::SetSpecularColor(float r, float g, float b, float a) {
	float specularColor[] = { r,g,b,a };
	glLightfv(mLightIdentifier, GL_SPECULAR, specularColor);
}
void Light::Enable() {
	glEnable(GL_LIGHTING);
	glEnable(mLightIdentifier);
}
DirectionLight::DirectionLight(GLenum light) {
	mLightIdentifier = light;
}
void DirectionLight::SetPosition(float x, float y, float z) {
	float pos[] = { x,y,z,0.0f };
	glLightfv(mLightIdentifier, GL_POSITION, pos);
}

PointLight::PointLight(GLenum light) {
	mLightIdentifier = light;
	memset(mPosition, 0, sizeof(mPosition));
}
void PointLight::SetPosition(float x, float y, float z) {
	mPosition[0] = x;
	mPosition[1] = y;
	mPosition[2] = z;
}
void PointLight::SetConstAttenuation(float v) {
	glLightf(mLightIdentifier, GL_CONSTANT_ATTENUATION, v);
}
void PointLight::SetLinearAttenuation(float v) {
	glLightf(mLightIdentifier, GL_LINEAR_ATTENUATION, v);
}
void PointLight::SetQuadricAttenuation(float v) {
	glLightf(mLightIdentifier, GL_QUADRATIC_ATTENUATION, v);
}
void PointLight::Update(float x, float y, float z) {
	float pos[] = { mPosition[0] - x, mPosition[1] - y, mPosition[2] - z,1.0f };
	glLightfv(mLightIdentifier, GL_POSITION, pos);
}
SpotLight::SpotLight(GLenum light) :PointLight(light) {
}
void SpotLight::SetDirection(float x, float y, float z) {
	float dir[] = { x,y,z,1.0f };
	glLightfv(mLightIdentifier, GL_SPOT_DIRECTION, dir);
}
void SpotLight::SetExponent(float v) {
	glLightf(mLightIdentifier, GL_SPOT_EXPONENT, v);
}
void SpotLight::SetCutoff(float v) {
	glLightf(mLightIdentifier, GL_SPOT_CUTOFF, v);
}