/*
BodySlide and Outfit Studio
Copyright (C) 2017  Caliente & ousnius
See the included LICENSE file
*/

#pragma once

#include "BasicTypes.h"
#include "Animation.h"
#include "ExtraData.h"

class NiObjectNET : public NiObject {
private:
	StringRef name;
	BlockRef<NiTimeController> controllerRef;
	BlockRefArray<NiExtraData> extraDataRefs;

public:
	uint skyrimShaderType = 0;				// BSLightingShaderProperty && User Version >= 12
	bool bBSLightingShaderProperty = false;

	void Get(NiStream& stream);
	void Put(NiStream& stream);
	void GetStringRefs(std::set<StringRef*>& refs);

	std::string GetName();
	void SetName(const std::string& str);
	void ClearName();

	int GetControllerRef();
	void SetControllerRef(int ctlrRef);

	int GetNumExtraData();
	void SetExtraDataRef(const int id, const int blockId);
	int GetExtraDataRef(const int id);
	void AddExtraDataRef(const int id);

	void GetChildRefs(std::set<int*>& refs);
};

class NiProperty;
class NiCollisionObject;

class NiAVObject : public NiObjectNET {
protected:
	BlockRef<NiCollisionObject> collisionRef;

public:
	uint flags = 524302;
	Vector3 translation;
	Vector3 rotation[3] = { Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) };
	float scale = 1.0f;
	BlockRefArray<NiProperty> propertyRefs;

	void Get(NiStream& stream);
	void Put(NiStream& stream);
	void GetChildRefs(std::set<int*>& refs);

	int GetCollisionRef() { return collisionRef.index; }
	void SetCollisionRef(const int colRef) { collisionRef.index = colRef; }

	bool rotToEulerDegrees(float &Y, float& P, float& R) {
		float rx, ry, rz;
		bool canRot = false;

		if (rotation[0].z < 1.0) {
			if (rotation[0].z > -1.0) {
				rx = atan2(-rotation[1].z, rotation[2].z);
				ry = asin(rotation[0].z);
				rz = atan2(-rotation[0].y, rotation[0].x);
				canRot = true;
			}
			else {
				rx = -atan2(-rotation[1].x, rotation[1].y);
				ry = -PI / 2;
				rz = 0.0f;
			}
		}
		else {
			rx = atan2(rotation[1].x, rotation[1].y);
			ry = PI / 2;
			rz = 0.0f;
		}

		Y = rx * 180 / PI;
		P = ry * 180 / PI;
		R = rz * 180 / PI;
		return canRot;
	}
};

struct AVObject {
	NiString name;
	BlockRef<NiAVObject> objectRef;
};

class NiAVObjectPalette : public NiObject {
};

class NiDefaultAVObjectPalette : public NiAVObjectPalette {
private:
	BlockRef<NiAVObject> sceneRef;
	uint numObjects = 0;
	std::vector<AVObject> objects;

public:
	NiDefaultAVObjectPalette() {}
	NiDefaultAVObjectPalette(NiStream& stream);

	static constexpr const char* BlockName = "NiDefaultAVObjectPalette";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);
	void GetPtrs(std::set<int*>& ptrs);
	NiDefaultAVObjectPalette* Clone() { return new NiDefaultAVObjectPalette(*this); }
};

class NiCamera : public NiAVObject {
private:
	ushort obsoleteFlags;
	float frustumLeft;
	float frustumRight;
	float frustumTop;
	float frustomBottom;
	float frustumNear;
	float frustumFar;
	bool useOrtho;
	float viewportLeft;
	float viewportRight;
	float viewportTop;
	float viewportBottom;
	float lodAdjust;

	BlockRef<NiAVObject> sceneRef;
	uint numScreenPolygons = 0;
	uint numScreenTextures = 0;

public:
	NiCamera() {}
	NiCamera(NiStream& stream);

	static constexpr const char* BlockName = "NiCamera";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);
	void GetChildRefs(std::set<int*>& refs);
	NiCamera* Clone() { return new NiCamera(*this); }
};

enum PixelLayout : uint {
	PIX_LAY_PALETTISED,
	PIX_LAY_HIGH_COLOR_16,
	PIX_LAY_TRUE_COLOR_32,
	PIX_LAY_COMPRESSED,
	PIX_LAY_BUMPMAP,
	PIX_LAY_PALETTISED_4,
	PIX_LAY_DEFAULT
};

enum MipMapFormat : uint {
	MIP_FMT_NO,
	MIP_FMT_YES,
	MIP_FMT_DEFAULT
};

enum AlphaFormat : uint {
	ALPHA_NONE,
	ALPHA_BINARY,
	ALPHA_SMOOTH,
	ALPHA_DEFAULT
};

class NiTexture : public NiObjectNET {
};

class NiSourceTexture : public NiTexture {
private:
	bool useExternal = true;
	StringRef fileName;

	// NiPixelData if < 20.2.0.4 or !persistentRenderData
	// else NiPersistentSrcTextureRendererData
	BlockRef<NiObject> dataRef;

	PixelLayout pixelLayout = PIX_LAY_PALETTISED_4;
	MipMapFormat mipMapFormat = MIP_FMT_DEFAULT;
	AlphaFormat alphaFormat = ALPHA_DEFAULT;
	bool isStatic = true;
	bool directRender = true;
	bool persistentRenderData = false;

public:
	NiSourceTexture() {}
	NiSourceTexture(NiStream& stream);

	static constexpr const char* BlockName = "NiSourceTexture";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);
	void GetStringRefs(std::set<StringRef*>& refs);
	void GetChildRefs(std::set<int*>& refs);
	NiSourceTexture* Clone() { return new NiSourceTexture(*this); }
};

enum TexFilterMode : uint {
	FILTER_NEAREST,
	FILTER_BILERP,
	FILTER_TRILERP,
	FILTER_NEAREST_MIPNEAREST,
	FILTER_NEAREST_MIPLERP,
	FILTER_BILERP_MIPNEAREST
};

enum TexClampMode : uint {
	CLAMP_S_CLAMP_T,
	CLAMP_S_WRAP_T,
	WRAP_S_CLAMP_T,
	WRAP_S_WRAP_T
};

enum EffectType : uint {
	EFFECT_PROJECTED_LIGHT,
	EFFECT_PROJECTED_SHADOW,
	EFFECT_ENVIRONMENT_MAP,
	EFFECT_FOG_MAP
};

enum CoordGenType : uint {
	CG_WORLD_PARALLEL,
	CG_WORLD_PERSPECTIVE,
	CG_SPHERE_MAP,
	CG_SPECULAR_CUBE_MAP,
	CG_DIFFUSE_CUBE_MAP
};

class NiDynamicEffect : public NiAVObject {
private:
	bool switchState = false;
	BlockRefArray<NiNode> affectedNodes;

public:
	void Get(NiStream& stream);
	void Put(NiStream& stream);
	void GetChildRefs(std::set<int*>& refs);
};

class NiTextureEffect : public NiDynamicEffect {
private:
	Matrix3 modelProjectionMatrix;
	Vector3 modelProjectionTransform;
	TexFilterMode textureFiltering = FILTER_TRILERP;
	TexClampMode textureClamping = WRAP_S_WRAP_T;
	EffectType textureType = EFFECT_ENVIRONMENT_MAP;
	CoordGenType coordinateGenerationType = CG_SPHERE_MAP;
	BlockRef<NiSourceTexture> sourceTexture;
	byte clippingPlane = 0;
	Vector3 unkVector = Vector3(1.0f, 0.0f, 0.0f);
	float unkFloat = 0.0f;

public:
	NiTextureEffect() {}
	NiTextureEffect(NiStream& stream);

	static constexpr const char* BlockName = "NiTextureEffect";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);
	void GetChildRefs(std::set<int*>& refs);
	NiTextureEffect* Clone() { return new NiTextureEffect(*this); }
};

class NiLight : public NiDynamicEffect {
private:
	float dimmer = 0.0f;
	Color3 ambientColor;
	Color3 diffuseColor;
	Color3 specularColor;

public:
	void Get(NiStream& stream);
	void Put(NiStream& stream);
};

class NiAmbientLight : public NiLight {
public:
	NiAmbientLight() {}
	NiAmbientLight(NiStream& stream);

	static constexpr const char* BlockName = "NiAmbientLight";
	virtual const char* GetBlockName() { return BlockName; }

	NiAmbientLight* Clone() { return new NiAmbientLight(*this); }
};

class NiDirectionalLight : public NiLight {
public:
	NiDirectionalLight() {}
	NiDirectionalLight(NiStream& stream);

	static constexpr const char* BlockName = "NiDirectionalLight";
	virtual const char* GetBlockName() { return BlockName; }

	NiDirectionalLight* Clone() { return new NiDirectionalLight(*this); }
};

class NiPointLight : public NiLight {
private:
	float constantAttenuation = 0.0f;
	float linearAttenuation = 0.0f;
	float quadraticAttenuation = 0.0f;

public:
	NiPointLight() {}
	NiPointLight(NiStream& stream);

	static constexpr const char* BlockName = "NiPointLight";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);
	NiPointLight* Clone() { return new NiPointLight(*this); }
};

class NiSpotLight : public NiPointLight {
private:
	float cutoffAngle = 0.0f;
	float unkFloat = 0.0f;
	float exponent = 0.0f;

public:
	NiSpotLight() {}
	NiSpotLight(NiStream& stream);

	static constexpr const char* BlockName = "NiSpotLight";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);
	NiSpotLight* Clone() { return new NiSpotLight(*this); }
};
