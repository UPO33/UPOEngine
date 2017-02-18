#include "UEntityAudioSource.h"
#include "../Meta/UMeta.h"
#include "UAudioDevice.h"
#include "UEntityAudioSourceVisualizer.h"

namespace UPO
{
	UENUM(EAudioAttenuationShape, false, nullptr,
		EAudioAttenuationShape::ESphere,
		EAudioAttenuationShape::EBox);

	UCLASS_BEGIN_IMPL(EntityAudioSource, UATTR_Instanceable(), UATTR_Icon("Sound.png"))
		UPROPERTY(mVolume)
		UPROPERTY(mPitch)
		UPROPERTY(mLoop)
		UPROPERTY(mSound)
		UPROPERTY(mAttenuate)
		UPROPERTY(mAttenuationShape)
		UPROPERTY(mSphereRadius)
		UPROPERTY(mSphereFallof)
		UPROPERTY(mBoxSize)
		UPROPERTY(mBoxFallof)
	UCLASS_END_IMPL(EntityAudioSource)

	
		
	EntityAudioSource::EntityAudioSource()
	{
		mHandle = new AudioSourceAL;
		mVisualizer = new EntityAudioSourceVisualizer(this);
	}

	EntityAudioSource::~EntityAudioSource()
	{
		SafeDelete(mHandle);
	}

	void EntityAudioSource::SetVolume(float volume)
	{
		mVolume = volume;
		mHandle->SetVolume(volume);
	}

	void EntityAudioSource::SetPitch(float pitch)
	{
		mPitch = pitch;
		mHandle->SetPitch(pitch);
	}

	void EntityAudioSource::SetLooping(bool loop)
	{
		mLoop = loop;
		mHandle->SetLooping(loop);
	}

	bool EntityAudioSource::IsPlaying() const
	{
		return mHandle->IsPlaying();
	}

	void EntityAudioSource::Play()
	{
		mHandle->Play();
	}

	void EntityAudioSource::Stop()
	{
		mHandle->Stop();
	}

	void EntityAudioSource::Pause()
	{
		mHandle->Pause();
	}



	void EntityAudioSource::SetSound(ASound* sound)
	{
		mSound = sound;

		if (sound)
		{
			bool isPlaying = mHandle->IsPlaying();
			mHandle->Stop();
			mHandle->SetBuffer(*mSound->mAudio);
			if (isPlaying) mHandle->Play();
		}
		else
		{
			mHandle->Stop();
			mHandle->UnsetBuffer();
		}
		
		
	}

	void EntityAudioSource::MetaAfterPropertyChange(const PropertyInfo* prp)
	{
		SetVolume(mVolume);
		SetPitch(mPitch);
		SetLooping(mLoop);
		SetSound(mSound);

		if (mIsSelected && !IsPlaying())
			Play();
	}

	void EntityAudioSource::OnTransformChanged()
	{
		Parent::OnTransformChanged();

		mHandle->SetPosition(GetWorldPosition());
	}

	void EntityAudioSource::OnTick()
	{
		Parent::OnTick();

		if (mAttenuate)
			mAttenuation = CalcAttenation();
	}

	float EntityAudioSource::CalcAttenation(const Vec3& listenerPosition)
	{
		switch (mAttenuationShape)
		{
		case EAudioAttenuationShape::ESphere:

			return Clamp(((GetWorldPosition() - listenerPosition).Length() - mSphereRadius) / (mSphereFallof + 0.0001f), 0, 1);

		case EAudioAttenuationShape::EBox:

			//TODO not implemented
			return 1;
		}
		return 1;
	}

};