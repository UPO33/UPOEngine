#pragma once

#include "UEntityLight.h"
#include "UPrimitiveBatch.h"
#include "UGameWindow.h"
#include "UHitSelection.h"

namespace UPO
{
	class UAPI EntityLightVisualizer : public EntityVisualizer
	{
	public:

		EntityLightVisualizer(EntityLight* owner) : EntityVisualizer(owner) {}

		void OnVisualize(PrimitiveBatch* batch) override
		{
			static const Color32 DefaultColor = Color32::GREEN;
			static const Color32 SelectedColor = Color32::RED;

			EntityLight* owner = GetOwner<EntityLight>();
			if(owner && owner->IsAlive())
			{
				Color32 color = owner->mColor;
				batch->SetHitProxy(new HPEntity(owner, ECursorShape::Cross));
				batch->DrawWireSphere(mOwner->GetWorldPosition(), owner->mRadius, color);
			}
		}
	};
};