
#ifndef _ZEQ_STRUCTS_EQG_H
#define _ZEQ_STRUCTS_EQG_H

#include <string>
#include <unordered_map>

#include "types.h"
#include "structs_intermediate.h"

class TER;

namespace EQG_Structs
{
	struct Property;

	typedef void(*PropertyFunction)(Property*, IntermediateMaterialEntry&, TER*);
	//static std::unordered_map<std::string, PropertyFunction, std::hash<std::string>> PropertyFunctions;
#define PROP_FUNC(x) void x(Property* prop, IntermediateMaterialEntry& mat_ent, TER* ter)

	std::unordered_map<std::string, PropertyFunction, std::hash<std::string>>& getPropertyFunctions();
	void initialize();
	PROP_FUNC(e_TextureDiffuse0);

	struct Material
	{
		uint32 index;
		uint32 name_index;
		uint32 shader_index;
		uint32 property_count;
	};

	struct Property
	{
		uint32 name_index;
		uint32 type;
		union
		{
			uint32 i;
			float f;
		} value;
	};

	struct Vertex
	{
		float x, y, z;
		float i, j, k;
		float u, v;
	};

	struct VertexV3
	{
		float x, y, z;
		float i, j, k;
		uint32 color;
		float u, v;
		float unknown[2];
	};

	struct Triangle
	{
		enum Flags
		{
			PERMEABLE = 0x01 //not subject to collision
		};

		uint32 index[3];
		int material;
		uint32 flag;
	};
}

#endif
