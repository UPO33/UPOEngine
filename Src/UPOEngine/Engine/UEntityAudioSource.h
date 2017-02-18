#pragma once

#include "UEntity.h"
#include "USound.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class AudioSourceAL;

	//////////////////////////////////////////////////////////////////////////
	enum class EAudioAttenuationShape
	{
		ESphere,
		EBox,

		EMax,
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI EntityAudioSource : public Entity
	{
		UCLASS(EntityAudioSource, Entity)

		float					mVolume = 1;
		float					mPitch = 1;
		bool					mLoop = true;
		ASound*					mSound = nullptr;
		bool					mAttenuate = true;
		EAudioAttenuationShape	mAttenuationShape = EAudioAttenuationShape::ESphere;
		float					mSphereRadius = 32;
		float					mSphereFallof = 8;
		Vec3					mBoxSize = Vec3(32);
		Vec3					mBoxFallof = Vec3(8);



		AudioSourceAL*	mHandle = nullptr;

	public:
		TDelegateMulti<void, EntityAudioSource*>	mOnPlayFinished;

		EntityAudioSource();
		~EntityAudioSource();

		void SetVolume(float);
		float GetVolume() const { return mVolume; }
		void SetPitch(float);
		float GetPitch() const { return mPitch; }
		void SetLooping(bool);
		bool IsLooping() const { return mLoop; }
		bool IsPlaying() const;

		void Play();
		void Stop();
		void Pause();

		void SetSound(ASound*);
		ASound* GetSound() const { return mSound; }

		void MetaAfterPropertyChange(const PropertyInfo* prp);

		virtual void OnTransformChanged() override;
		virtual void OnTick() override;

		float CalcAttenation(const Vec3& listenerPosition);

	};
};