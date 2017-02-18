#pragma once

#include "UEntityStaticMesh.h"
#include "UPrimitiveBatch.h"
#include "UGameWindow.h"
#include "UHitSelection.h"

namespace UPO
{
	class UAPI EntityStaticMeshVisualizer : public EntityVisualizer
	{
	public:

		EntityStaticMeshVisualizer(EntityStaticMesh* owner) : EntityVisualizer(owner) {}

		void OnVisualize(PrimitiveBatch* batch) override
		{
			static const Color32 DefaultColor = Color32::GREEN;
			static const Color32 SelectedColor = Color32::RED;

			if (mOwner->mIsSelected || GetGameWindow()->mOptions.mShowBounds)
			{
				Color32 color = mOwner->mIsSelected ? SelectedColor : DefaultColor;
				batch->SetHitProxy(new HPEntity(mOwner));
				batch->DrawWireBox(mOwner->GetBound().mMin, mOwner->GetBound().mMax, color);
				float radius = (mOwner->GetBound().mMin - mOwner->GetBound().mMax).Length() / 2;
				batch->DrawWireSphere(mOwner->GetBound().GetCenter(), radius, color);
			}
		}
	};
};