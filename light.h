#pragma once
#include"GL/freeglut.h"
#include<string.h>

// 光源类

class Light {
protected:
	GLenum mLightIdentifier;       // GL_LIGHT(0,1,2……7)
	Light();
public:
	void SetAmbientColor(float r, float g, float b, float a);    // 设置 ambient  材质 
	void SetDiffuseColor(float r, float g, float b, float a);	 // 设置 diffuse  材质
	void SetSpecularColor(float r, float g, float b, float a);	 // 设置 specular 材质
	void Enable();         // GL_ENABLE
};
class DirectionLight :public Light {
public:
	DirectionLight(GLenum light);                              
	void SetPosition(float x, float y, float z);                 // 设置位置   // 方向光，位置即方向
};
class PointLight :public Light {
	float mPosition[3];
public:
	PointLight(GLenum light);         
	void SetPosition(float x, float y, float z);                 // 设置位置
	void SetConstAttenuation(float v);                          // 设置 衰减    常数衰减
	void SetLinearAttenuation(float v);							// 设置 衰减    线性衰减
	void SetQuadricAttenuation(float v);						// 设置 衰减    平方衰减
	void Update(float x, float y, float z);                      //  更新
};
class SpotLight :public PointLight {
public:
	SpotLight(GLenum light);
	void SetDirection(float x, float y, float z);           // 设置方向
	void SetExponent(float v);                               //  设置聚焦指数
	void SetCutoff(float v);                              // 设置最大发散角
};