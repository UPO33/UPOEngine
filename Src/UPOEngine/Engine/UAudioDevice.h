#pragma once

#include <al.h>
#include <alc.h>

#include "../Core/UVector.h"


namespace UPO
{
	class AudioDevice
	{
	public:
		ALCdevice*	mDevice = nullptr;
		ALCcontext*	mContext = nullptr;

		AudioDevice()
		{
			mDevice = alcOpenDevice(nullptr);
			UASSERT(mDevice);
			mContext = ::alcCreateContext(mDevice, nullptr);
			UASSERT(mContext);
			::alcMakeContextCurrent(mContext);
			::alDistanceModel(AL_NONE); 
			
		}
		~AudioDevice()
		{
			if (mContext) 
			{
				::alcMakeContextCurrent(nullptr);
				::alcDestroyContext(mContext);
				::alcCloseDevice(mDevice);
			}

		}
		void SetListenerPositionOrientation(const Vec3& position, const Vec3& forward, const Vec3& up)
		{
			float posRH[3] = { position.mX, position.mY, -position.mZ };
			float orientation[] = { forward.mX, forward.mY, -forward.mZ, up.mX, up.mY, -up.mZ };
			float velocity[] = { 0,0,0 };
			::alListenerfv(AL_POSITION, posRH);
			::alListenerfv(AL_ORIENTATION, orientation);
			::alListenerfv(AL_VELOCITY, velocity);

		}

		float GetSpeedOfSound()
		{
			return alGetFloat(AL_SPEED_OF_SOUND);
		}
		void SetSpeedOfSound(float value)
		{
			alSpeedOfSound(value);
		}

	};

	//////////////////////////////////////////////////////////////////////////
	class AudioBufferAL
	{
	public:
		ALuint mHandle = 0;
		AudioBufferAL()
		{
			::alGenBuffers(1, &mHandle);
		}
		AudioBufferAL(ALenum format, ALvoid* data, ALsizei size, ALsizei freg)
		{
			::alGenBuffers(1, &mHandle);
			::alBufferData(mHandle, format, data, size, freg);
		}
		~AudioBufferAL()
		{
			::alDeleteBuffers(1, &mHandle);
		}
		//Frequency, specified in samples per second
		int GetFrequency()
		{
			ALint value = 0;
			::alGetBufferi(mHandle, AL_FREQUENCY, &value);
			return value;
		}
		//Size in bytes of the buffer data
		int GetSize()
		{
			ALint value = 0;
			::alGetBufferi(mHandle, AL_SIZE, &value);
			return value;
		}
		//The number of bits per sample for the data contained in the buffer
		int GetBitsPerSample()
		{
			ALint value = 0;
			::alGetBufferi(mHandle, AL_BITS, &value);
			return value;
		}
		//The number of channels for the data contained in the buffer
		int GetNumChannels()
		{
			ALint value = 0;
			::alGetBufferi(mHandle, AL_CHANNELS, &value);
			return value;
		}
	};


	//////////////////////////////////////////////////////////////////////////
	class AudioSourceAL
	{
	public:
		ALuint	mHandle;

		AudioSourceAL()
		{
			::alGenSources(1, &mHandle);
		}
		~AudioSourceAL()
		{
			::alDeleteSources(1, &mHandle);
		}
		void SetPosition(const Vec3& position)
		{
			float posRH[3] = { position.mX, position.mY, -position.mZ };
			::alSourcefv(mHandle, AL_POSITION, posRH);
		}
		void Play()
		{
			::alSourcePlay(mHandle);
		}
		void Stop()
		{
			::alSourceStop(mHandle);
		}
		void Pause()
		{
			::alSourcePause(mHandle);
		}
		void Rewind()
		{
			::alSourceRewind(mHandle);
		}
		void SetLooping(bool loop)
		{
			::alSourcei(mHandle, AL_LOOPING, loop ? 1 : 0);
		}
		void SetVolume(float volume)
		{
			::alSourcef(mHandle, AL_GAIN, volume);
		}
		void SetPitch(float pitch)
		{
			::alSourcef(mHandle, AL_PITCH, pitch);
		}
		bool IsPlaying()
		{
			ALint value = 0;
			alGetSourcei(mHandle, AL_SOURCE_STATE, &value);
			return value == AL_PLAYING;
		}
		void SetBuffer(AudioBufferAL buffer)
		{
			::alSourcei(mHandle, AL_BUFFER, buffer.mHandle);
		}
		void UnsetBuffer()
		{
			::alSourcei(mHandle, AL_BUFFER, 0);
		}
		void SetPlaybackPosition(float seconds)
		{
			::alSourcef(mHandle, AL_SEC_OFFSET, seconds);
		}
		float GetPlayBackPosition()
		{
			float value = 0;
			::alGetSourcef(mHandle, AL_DIRECTION, &value);
			return value;
		}
	};


};