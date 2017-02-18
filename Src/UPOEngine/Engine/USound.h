#pragma once

#include "UAsset.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class AudioBufferAL;

	//////////////////////////////////////////////////////////////////////////
	class UAPI ASound : public Asset
	{
		friend UPOEd::AssetConverter;

		UCLASS(ASound, Asset)

		Buffer	mContent;
		AudioBufferAL*	mAudio;

		unsigned	mNumChannels;
		unsigned	mBitsPerSample;
		unsigned	mFrequency;
		float		mDuration;
		
	public:
		ASound();
		~ASound();

	protected:
		void OnCreate() override;
		void OnDestroy() override;
	};
};