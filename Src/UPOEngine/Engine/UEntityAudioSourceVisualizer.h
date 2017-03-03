#pragma once

#include "UEntityAudioSource.h"
#include "UPrimitiveBatch.h"
#include "UWorld.h"
#include "UGameWindow.h"
#include "UHitSelection.h"


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class EntityAudioSourceVisualizer : public EntityVisualizer
	{
	public:
		EntityAudioSourceVisualizer(EntityAudioSource* owner) : EntityVisualizer(owner) {}

		void OnVisualize(PrimitiveBatch* pb) override
		{
			EntityAudioSource* owner = GetOwner<EntityAudioSource>();

			if(owner && owner->IsAlive())
			{
				pb->SetHitProxy(new HPEntity(owner, ECursorShape::Cross));
				Color32 color = owner->mIsSelected ? Color32::RED : Color32::BLUE;
				if (owner->mAttenuationShape == EAudioAttenuationShape::ESphere)
				{
					pb->DrawWireSphere(owner->GetWorldPosition(), owner->mSphereRadius, color);
					pb->DrawWireSphere(owner->GetWorldPosition(), owner->mSphereRadius + owner->mSphereFallof, color);
				}
				else if (owner->mAttenuationShape == EAudioAttenuationShape::EBox)
				{
					pb->DrawWireBox(owner->mBoxSize, owner->GetWorldTransform(), color);
					pb->DrawWireBox(owner->mBoxSize + owner->mBoxFallof, owner->GetWorldTransform(), color);
				}
			}
		}
	};
};