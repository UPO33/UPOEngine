#include "USound.h"
#include "../Meta/UMeta.h"
#include "UAudioDevice.h"
#include "../Misc/UWAVLoader.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(ASound, UATTR_Icon("Sound.png"))
		UPROPERTY(mContent, UATTR_Hidden())
		UPROPERTY(mNumChannels, UATTR_Uneditable())
		UPROPERTY(mBitsPerSample, UATTR_Uneditable())
		UPROPERTY(mFrequency, UATTR_Uneditable())
		UPROPERTY(mDuration, UATTR_Uneditable())
	UCLASS_END_IMPL(ASound)





		ASound::ASound()
	{

	}

	ASound::~ASound()
	{

	}
	const char* UALSoundFormatToStr(unsigned in)
	{
		switch (in)
		{
		case AL_FORMAT_MONO8: return "Mono8";
		case AL_FORMAT_MONO16:return "Mono16";
		case AL_FORMAT_STEREO8:return "Stereo8";
		case AL_FORMAT_STEREO16:return "Stereo16";
		}
		return "";
	}
	void ASound::OnCreate()
	{
		File file = File("tmpaudio", EFileOpenMode::Write);
		file.WriteBytes(mContent.Data(), mContent.Size());
		file.Close();

		WAVEID wid = 0;
		if (CWavesLoader::Get()->LoadWaveFile("tmpaudio", &wid) == WAVERESULT::WR_OK)
		{
			unsigned long freg = 0;
			unsigned long size = 0;
			unsigned long format = 0;
			void* data = nullptr;

			CWavesLoader::Get()->GetWaveFrequency(wid, &freg);
			CWavesLoader::Get()->GetWaveSize(wid, &size);
			CWavesLoader::Get()->GetWaveData(wid, &data);
			CWavesLoader::Get()->GetWaveALBufferFormat(wid, &alGetEnumValue, &format);
			ULOG_SUCCESS("freg % size % format %", freg, size, format);

			mAudio = new AudioBufferAL(format, data, size, freg);
			
			CWavesLoader::Get()->DeleteWaveFile(wid);

			mNumChannels = mAudio->GetNumChannels();
			mBitsPerSample = mAudio->GetBitsPerSample();
			mFrequency = mAudio->GetFrequency();

			mDuration = ((double)size) / (freg*mNumChannels*(mBitsPerSample / 8));
		}
	}

	void ASound::OnDestroy()
	{
		SafeDelete(mAudio);
	}

};
