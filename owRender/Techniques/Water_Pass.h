#pragma once

#include "technique.h"

class Water_Pass : public Technique
{
public:
	Water_Pass();
	virtual bool Init();

	inline void SetColorTextureUnit(int TextureUnit)
	{
		setTexture("gColorMap", TextureUnit);
	}

	inline void SetSpecularTextureUnit(int TextureUnit)
	{
		setTexture("gSpecularMap", TextureUnit);
	}

	void SetWaterColorLight(vec3 _Color)
	{
		setVec3("gColorLight", _Color);
	}

	void SetWaterColorDark(vec3 _Color)
	{
		setVec3("gColorDark", _Color);
	}
};