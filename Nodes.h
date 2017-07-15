/*
BodySlide and Outfit Studio
Copyright (C) 2017  Caliente & ousnius
See the included LICENSE file
*/

#pragma once

#include "BasicTypes.h"
#include "Objects.h"

class NiNode : public NiAVObject {
private:
	BlockRefArray<NiAVObject> childRefs;
	BlockRefArray<NiAVObject> effectRefs;	// should be NiDynamicEffect

public:
	NiNode() {}
	NiNode(NiStream& stream);

	static constexpr const char* BlockName = "NiNode";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);

	void GetChildRefs(std::set<int*>& refs);
	NiNode* Clone() { return new NiNode(*this); }

	int GetNumChildren() { return childRefs.GetSize(); }
	int GetChildRef(const int id);
	void AddChildRef(const int id);
	void ClearChildren();
	std::vector<BlockRef<NiAVObject>>& GetChildren() { return childRefs.GetBlockRefs(); }

	int GetNumEffects() { return effectRefs.GetSize(); }
	int GetEffectRef(const int id);
	void AddEffectRef(const int id);
};

class BSFadeNode : public NiNode {
public:
	BSFadeNode();
	BSFadeNode(NiStream& stream);

	static constexpr const char* BlockName = "BSFadeNode";
	virtual const char* GetBlockName() { return BlockName; }

	BSFadeNode* Clone() { return new BSFadeNode(*this); }
};

enum BSValueNodeFlags : byte {
	BSVN_NONE = 0x0,
	BSVN_BILLBOARD_WORLD_Z = 0x1,
	BSVN_USE_PLAYER_ADJUST = 0x2
};

class BSValueNode : public NiNode {
private:
	int value = 0;
	BSValueNodeFlags valueFlags = BSVN_NONE;

public:
	BSValueNode();
	BSValueNode(NiStream& stream);

	static constexpr const char* BlockName = "BSValueNode";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);

	BSValueNode* Clone() { return new BSValueNode(*this); }
};

class BSLeafAnimNode : public NiNode {
public:
	BSLeafAnimNode();
	BSLeafAnimNode(NiStream& stream);

	static constexpr const char* BlockName = "BSLeafAnimNode";
	virtual const char* GetBlockName() { return BlockName; }
};

class BSTreeNode : public NiNode {
private:
	BlockRefArray<NiNode> bones1;
	BlockRefArray<NiNode> bones2;

public:
	BSTreeNode();
	BSTreeNode(NiStream& stream);

	static constexpr const char* BlockName = "BSTreeNode";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);

	void GetChildRefs(std::set<int*>& refs);
	BSTreeNode* Clone() { return new BSTreeNode(*this); }
};

class BSOrderedNode : public NiNode {
private:
	Vector4 alphaSortBound;
	bool isStaticBound = false;

public:
	BSOrderedNode();
	BSOrderedNode(NiStream& stream);

	static constexpr const char* BlockName = "BSOrderedNode";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);

	BSOrderedNode* Clone() { return new BSOrderedNode(*this); }
};

class BSMultiBoundData : public NiObject {
};

class BSMultiBoundOBB : public BSMultiBoundData {
private:
	Vector3 center;
	Vector3 size;
	float rotation[9] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

public:
	BSMultiBoundOBB() {}
	BSMultiBoundOBB(NiStream& stream);

	static constexpr const char* BlockName = "BSMultiBoundOBB";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);
	BSMultiBoundOBB* Clone() { return new BSMultiBoundOBB(*this); }
};

class BSMultiBoundAABB : public BSMultiBoundData {
private:
	Vector3 center;
	Vector3 halfExtent;

public:
	BSMultiBoundAABB() {}
	BSMultiBoundAABB(NiStream& stream);

	static constexpr const char* BlockName = "BSMultiBoundAABB";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);
	BSMultiBoundAABB* Clone() { return new BSMultiBoundAABB(*this); }
};

class BSMultiBoundSphere : public BSMultiBoundData {
private:
	Vector3 center;
	float radius = 0.0f;

public:
	BSMultiBoundSphere() {}
	BSMultiBoundSphere(NiStream& stream);

	static constexpr const char* BlockName = "BSMultiBoundSphere";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);
	BSMultiBoundSphere* Clone() { return new BSMultiBoundSphere(*this); }
};

class BSMultiBound : public NiObject {
private:
	BlockRef<BSMultiBoundData> dataRef;

public:
	BSMultiBound() {}
	BSMultiBound(NiStream& stream);

	static constexpr const char* BlockName = "BSMultiBound";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);
	void GetChildRefs(std::set<int*>& refs);
	BSMultiBound* Clone() { return new BSMultiBound(*this); }
};

enum BSCPCullingType : uint {
	BSCP_CULL_NORMAL,
	BSCP_CULL_ALLPASS,
	BSCP_CULL_ALLFAIL,
	BSCP_CULL_IGNOREMULTIBOUNDS,
	BSCP_CULL_FORCEMULTIBOUNDSNOUPDATE
};

class BSMultiBoundNode : public NiNode {
private:
	BlockRef<BSMultiBound> multiBoundRef;
	BSCPCullingType cullingMode = BSCP_CULL_NORMAL;

public:
	BSMultiBoundNode();
	BSMultiBoundNode(NiStream& stream);

	static constexpr const char* BlockName = "BSMultiBoundNode";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);

	void GetChildRefs(std::set<int*>& refs);
	BSMultiBoundNode* Clone() { return new BSMultiBoundNode(*this); }
};

class BSRangeNode : public NiNode {
private:
	byte min = 0;
	byte max = 0;
	byte current = 0;

public:
	BSRangeNode();
	BSRangeNode(NiStream& stream);

	static constexpr const char* BlockName = "BSRangeNode";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);

	BSRangeNode* Clone() { return new BSRangeNode(*this); }
};

class BSDebrisNode : public BSRangeNode {
public:
	BSDebrisNode();
	BSDebrisNode(NiStream& stream);

	static constexpr const char* BlockName = "BSDebrisNode";
	virtual const char* GetBlockName() { return BlockName; }

	BSDebrisNode* Clone() { return new BSDebrisNode(*this); }
};

class BSBlastNode : public BSRangeNode {
public:
	BSBlastNode();
	BSBlastNode(NiStream& stream);

	static constexpr const char* BlockName = "BSBlastNode";
	virtual const char* GetBlockName() { return BlockName; }

	BSBlastNode* Clone() { return new BSBlastNode(*this); }
};

class BSDamageStage : public BSBlastNode {
public:
	BSDamageStage();
	BSDamageStage(NiStream& stream);

	static constexpr const char* BlockName = "BSDamageStage";
	virtual const char* GetBlockName() { return BlockName; }

	BSDamageStage* Clone() { return new BSDamageStage(*this); }
};

enum BillboardMode : ushort {
	ALWAYS_FACE_CAMERA,
	ROTATE_ABOUT_UP,
	RIGID_FACE_CAMERA,
	ALWAYS_FACE_CENTER,
	RIGID_FACE_CENTER,
	BSROTATE_ABOUT_UP,
	ROTATE_ABOUT_UP2 = 9
};

class NiBillboardNode : public NiNode {
private:
	BillboardMode billboardMode = ALWAYS_FACE_CAMERA;

public:
	NiBillboardNode();
	NiBillboardNode(NiStream& stream);

	static constexpr const char* BlockName = "NiBillboardNode";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);

	NiBillboardNode* Clone() { return new NiBillboardNode(*this); }
};

enum NiSwitchFlags : ushort {
	UPDATE_ONLY_ACTIVE_CHILD,
	UPDATE_CONTROLLERS
};

class NiSwitchNode : public NiNode {
private:
	NiSwitchFlags flags = UPDATE_ONLY_ACTIVE_CHILD;
	uint index = 0;

public:
	NiSwitchNode();
	NiSwitchNode(NiStream& stream);

	static constexpr const char* BlockName = "NiSwitchNode";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);

	NiSwitchNode* Clone() { return new NiSwitchNode(*this); }
};

struct LODRange {
	float nearExtent = 0.0f;
	float farExtent = 0.0f;
};

class NiLODData : public NiObject {
};

class NiRangeLODData : public NiLODData {
private:
	Vector3 lodCenter;
	uint numLODLevels = 0;
	std::vector<LODRange> lodLevels;

public:
	NiRangeLODData();
	NiRangeLODData(NiStream& stream);

	static constexpr const char* BlockName = "NiRangeLODData";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);

	NiRangeLODData* Clone() { return new NiRangeLODData(*this); }
};

class NiScreenLODData : public NiLODData {
private:
	Vector3 boundCenter;
	float boundRadius = 0.0f;
	Vector3 worldCenter;
	float worldRadius = 0.0f;
	uint numProportions = 0;
	std::vector<float> proportionLevels;

public:
	NiScreenLODData();
	NiScreenLODData(NiStream& stream);

	static constexpr const char* BlockName = "NiScreenLODData";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);

	NiScreenLODData* Clone() { return new NiScreenLODData(*this); }
};

class NiLODNode : public NiSwitchNode {
private:
	BlockRef<NiLODData> lodLevelData;

public:
	NiLODNode();
	NiLODNode(NiStream& stream);

	static constexpr const char* BlockName = "NiLODNode";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);

	void GetChildRefs(std::set<int*>& refs);
	NiLODNode* Clone() { return new NiLODNode(*this); }
};

class NiBone : public NiNode {
public:
	NiBone();
	NiBone(NiStream& stream);

	static constexpr const char* BlockName = "NiBone";
	virtual const char* GetBlockName() { return BlockName; }

	NiBone* Clone() { return new NiBone(*this); }
};

enum SortingMode {
	SORTING_INHERIT,
	SORTING_OFF
};

class NiSortAdjustNode : public NiNode {
private:
	SortingMode sortingMode = SORTING_INHERIT;

public:
	NiSortAdjustNode();
	NiSortAdjustNode(NiStream& stream);

	static constexpr const char* BlockName = "NiSortAdjustNode";
	virtual const char* GetBlockName() { return BlockName; }

	void Get(NiStream& stream);
	void Put(NiStream& stream);

	NiSortAdjustNode* Clone() { return new NiSortAdjustNode(*this); }
};
