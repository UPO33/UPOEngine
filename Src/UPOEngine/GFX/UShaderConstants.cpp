#include "UShaderConstants.h"

#define IMPLSHADERCONSTANT(Varible) UAPI ShaderConstant Varible(#Varible, "1")


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	namespace ShaderConstants
	{
		IMPLSHADERCONSTANT(PIXEL_SHADER);
		IMPLSHADERCONSTANT(VERTEX_SHADER);
		IMPLSHADERCONSTANT(HULL_SHADER);
		IMPLSHADERCONSTANT(DOMAIN_SHADER);
		IMPLSHADERCONSTANT(GEOMERTY_SHADER);
		IMPLSHADERCONSTANT(COMPUTE_SHADER);
		IMPLSHADERCONSTANT(STATIC_MESH);
		IMPLSHADERCONSTANT(USE_HITSELECTION);
		IMPLSHADERCONSTANT(VISALIZE_GBUFFER);
	};
}