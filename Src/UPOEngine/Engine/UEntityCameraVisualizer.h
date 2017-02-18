#pragma 

#include "UEntityCamera.h"
#include "UPrimitiveBatch.h"
#include "UWorld.h"
#include "UGameWindow.h"
#include "UHitSelection.h"

namespace UPO
{
	class UAPI EntityCameraVisualizer : public EntityVisualizer
	{
	public:
		EntityCameraVisualizer(EntityCamera* camera) : EntityVisualizer(camera) {}

		void OnVisualize(PrimitiveBatch* batch) override
		{
			if (!mOwner->IsAlive())return;

			Color32 color = mOwner->mIsSelected ? Color32::RED : Color32::GREEN;


			if (mOwner->mIsSelected || GetWorld()->GetGameWindow()->mOptions.mShowBounds)
			{
				EntityCameraRS* rs = mOwner->GetRS<EntityCameraRS>();
				if (rs)
				{

					batch->SetHitProxy(new HPEntity(mOwner));
					batch->DrawWireFrustum(rs->mMatrixClipToWorld, color);
					batch->SetHitProxy(nullptr);
				}
			}
		}
	};
};