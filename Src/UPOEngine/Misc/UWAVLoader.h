#pragma once

#include "../Core/UBasic.h"
#include "../Core/UStream.h"
#include "../Core/UMemory.h"

#include <windows.h>
#include <stdio.h>

#define MAX_NUM_WAVEID			1024



	enum WAVEFILETYPE
	{
		WF_EX = 1,
		WF_EXT = 2
	};

	enum WAVERESULT
	{
		WR_OK = 0,
		WR_INVALIDFILENAME = -1,
		WR_BADWAVEFILE = -2,
		WR_INVALIDPARAM = -3,
		WR_INVALIDWAVEID = -4,
		WR_NOTSUPPORTEDYET = -5,
		WR_WAVEMUSTBEMONO = -6,
		WR_WAVEMUSTBEWAVEFORMATPCM = -7,
		WR_WAVESMUSTHAVESAMEBITRESOLUTION = -8,
		WR_WAVESMUSTHAVESAMEFREQUENCY = -9,
		WR_WAVESMUSTHAVESAMEBITRATE = -10,
		WR_WAVESMUSTHAVESAMEBLOCKALIGNMENT = -11,
		WR_OFFSETOUTOFDATARANGE = -12,
		WR_FILEERROR = -13,
		WR_OUTOFMEMORY = -14,
		WR_INVALIDSPEAKERPOS = -15,
		WR_INVALIDWAVEFILETYPE = -16,
		WR_NOTWAVEFORMATEXTENSIBLEFORMAT = -17
	};

#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_
	typedef struct tWAVEFORMATEX
	{
		WORD    wFormatTag;
		WORD    nChannels;
		DWORD   nSamplesPerSec;
		DWORD   nAvgBytesPerSec;
		WORD    nBlockAlign;
		WORD    wBitsPerSample;
		WORD    cbSize;
	} WAVEFORMATEX;
#endif /* _WAVEFORMATEX_ */

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_
	typedef struct {
		WAVEFORMATEX    Format;
		union {
			WORD wValidBitsPerSample;       /* bits of precision  */
			WORD wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
			WORD wReserved;                 /* If neither applies, set to zero. */
		} Samples;
		DWORD           dwChannelMask;      /* which channels are */
											/* present in stream  */
		GUID            SubFormat;
	} WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
#endif // !_WAVEFORMATEXTENSIBLE_

	typedef struct
	{
		WAVEFILETYPE	wfType;
		WAVEFORMATEXTENSIBLE wfEXT;		// For non-WAVEFORMATEXTENSIBLE wavefiles, the header is stored in the Format member of wfEXT
		char			*pData;
		unsigned long	ulDataSize;
		FILE			*pFile;
		unsigned long	ulDataOffset;
	} WAVEFILEINFO, *LPWAVEFILEINFO;

	typedef int(__cdecl *PFNALGETENUMVALUE)(const char *szEnumName);
	typedef int	WAVEID;

	//////////////////////////////////////////////////////////////////////////
	class  CWavesLoader
	{
	public:
		CWavesLoader();
		virtual ~CWavesLoader();

		static CWavesLoader* Get();

		WAVERESULT LoadWaveFile(const char *szFilename, WAVEID *WaveID);
		WAVERESULT OpenWaveFile(const char *szFilename, WAVEID *WaveID);
		WAVERESULT ReadWaveData(WAVEID WaveID, void *pData, unsigned long ulDataSize, unsigned long *pulBytesWritten);
		WAVERESULT SetWaveDataOffset(WAVEID WaveID, unsigned long ulOffset);
		WAVERESULT GetWaveDataOffset(WAVEID WaveID, unsigned long *pulOffset);
		WAVERESULT GetWaveType(WAVEID WaveID, WAVEFILETYPE *pwfType);
		WAVERESULT GetWaveFormatExHeader(WAVEID WaveID, WAVEFORMATEX *pWFEX);
		WAVERESULT GetWaveFormatExtensibleHeader(WAVEID WaveID, WAVEFORMATEXTENSIBLE *pWFEXT);
		WAVERESULT GetWaveData(WAVEID WaveID, void **ppAudioData);
		WAVERESULT GetWaveSize(WAVEID WaveID, unsigned long *pulDataSize);
		WAVERESULT GetWaveFrequency(WAVEID WaveID, unsigned long *pulFrequency);
		WAVERESULT GetWaveALBufferFormat(WAVEID WaveID, PFNALGETENUMVALUE pfnGetEnumValue, unsigned long *pulFormat);
		WAVERESULT DeleteWaveFile(WAVEID WaveID);

		char *GetErrorString(WAVERESULT wr, char *szErrorString, unsigned long nSizeOfErrorString);
		bool IsWaveID(WAVEID WaveID);

	private:
		WAVERESULT ParseFile(const char *szFilename, LPWAVEFILEINFO pWaveInfo);
		WAVERESULT ParseFile(UPO::Stream& stream, LPWAVEFILEINFO pOutWaveInfo);

		LPWAVEFILEINFO	m_WaveIDs[MAX_NUM_WAVEID];
	};

	/*
	ALboolean ALFWLoadWaveToBuffer(const char *szWaveFile, ALuint uiBufferID, ALenum eXRAMBufferMode)
	{
		WAVEID			WaveID;
		ALint			iDataSize, iFrequency;
		ALenum			eBufferFormat;
		ALchar			*pData;
		ALboolean		bReturn;

		bReturn = AL_FALSE;
		if (g_pWaveLoader)
		{
			if (SUCCEEDED(g_pWaveLoader->LoadWaveFile(szWaveFile, &WaveID)))
			{
				if ((SUCCEEDED(g_pWaveLoader->GetWaveSize(WaveID, (unsigned long*)&iDataSize))) &&
					(SUCCEEDED(g_pWaveLoader->GetWaveData(WaveID, (void**)&pData))) &&
					(SUCCEEDED(g_pWaveLoader->GetWaveFrequency(WaveID, (unsigned long*)&iFrequency))) &&
					(SUCCEEDED(g_pWaveLoader->GetWaveALBufferFormat(WaveID, &alGetEnumValue, (unsigned long*)&eBufferFormat))))
				{
					// Set XRAM Mode (if application)
					if (eaxSetBufferMode && eXRAMBufferMode)
						eaxSetBufferMode(1, &uiBufferID, eXRAMBufferMode);

					alGetError();
					alBufferData(uiBufferID, eBufferFormat, pData, iDataSize, iFrequency);
					if (alGetError() == AL_NO_ERROR)
						bReturn = AL_TRUE;

					g_pWaveLoader->DeleteWaveFile(WaveID);
				}
			}
		}

		return bReturn;
	}
	*/