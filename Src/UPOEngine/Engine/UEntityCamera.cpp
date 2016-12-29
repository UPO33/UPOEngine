#include "UEntityCamera.h"
#include "../Meta/UMeta.h"


namespace UPO
{
	  

	UCLASS_BEGIN_IMPL(EntityCamera)
	UCLASS_END_IMPL(EntityCamera)

	EntityCamera::EntityCamera()
	{
		mProjection = ECameraProjection::Perspective;
		mFieldOfView = 70;
		mNearClip = 0.01f;
		mFarClip = 10000.0f;
		mOrthoWidth = 100;
		mOrthoHeight = 1000;
	}


	EntityCamera::~EntityCamera()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	EntityCameraRS::EntityCameraRS(EntityCamera* from, WorldRS* wrs)
	{
		mWorldRS = wrs;
		mGS = from;

	}
	//////////////////////////////////////////////////////////////////////////
	EntityCameraRS::~EntityCameraRS()
	{

	}

	void EntityCameraRS::OnFetch()
	{

	}

};