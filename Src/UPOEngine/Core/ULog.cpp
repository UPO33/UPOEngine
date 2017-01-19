#include "ULog.h"
#include "UThreading.h"
#include "UFileSys.h"
#include "UString.h"
#include "UQueue.h"
#include <stdio.h>

//helper to show the colorized messages in console
namespace Console
{
#ifdef UPLATFORM_WIN
	static WORD BACKGROUND_COLOR = 0; // black

	namespace Dark
	{
		static const WORD BLUE = 1 + BACKGROUND_COLOR * 16;
		static const WORD GREEN = 2 + BACKGROUND_COLOR * 16;
		static const WORD CYAN = 3 + BACKGROUND_COLOR * 16;
		static const WORD RED = 4 + BACKGROUND_COLOR * 16;
		static const WORD PINK = 5 + BACKGROUND_COLOR * 16;
		static const WORD YELLOW = 6 + BACKGROUND_COLOR * 16;
		static const WORD WHITE = 7 + BACKGROUND_COLOR * 16;
		static const WORD GRAY = 8 + BACKGROUND_COLOR * 16;
	}

	namespace Bright
	{
		static const WORD BLUE = 9 + BACKGROUND_COLOR * 16;
		static const WORD GREEN = 10 + BACKGROUND_COLOR * 16;
		static const WORD CYAN = 11 + BACKGROUND_COLOR * 16;
		static const WORD RED = 12 + BACKGROUND_COLOR * 16;
		static const WORD PINK = 13 + BACKGROUND_COLOR * 16;
		static const WORD YELLOW = 14 + BACKGROUND_COLOR * 16;
		static const WORD WHITE = 15 + BACKGROUND_COLOR * 16;
	}

	void SetColor(int color)
	{
		HANDLE  hConsole;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
	}
	void MatchColor(UPO::ELogType logType)
	{
		int LUTColors[UPO::ELogType::ELT_Max] =
		{
			Console::Dark::WHITE, Console::Bright::GREEN, Console::Bright::YELLOW, Console::Bright::RED, Console::Bright::RED, Console::Bright::RED
		};
		Console::SetColor(LUTColors[logType]);
	}
#else
	void MatchColor(UPO::ELogType logType)
	{
		//TODO
	}
#endif
};



namespace UPO
{

	const char* gLogHTMLBegin =
		"<!DOCTYPE html><html><head><style>"
		"h1, h2{ color: white;	margin: 0; }"
		"p{ font - size: 110 % ; }"
		"p.message{ color: white; }"
		"p.error{ color: red; }"
		"p.success{ color: #66ff33; }"
		"p.warn{ color: yellow; }"
		"p.fatal{ color: red; }"
		"body{ background-color : black; }"
		"</style></head><body>"
		"<h1>UPOEngine log file</h1>"
		"<h2>[File Name] [Function Name] [LineNumber] [ThreadID] log</h2>"
		"<p></p>";

	const char* gLogHTMLEnd =
		"</body></html>";

	//////////////////////////////////////////////////////////////////////////
	void UGetLegibleThreadName(unsigned threadID, char outThreadName[32])
	{
		if (threadID == gGameThreadID)
			StrCopy(outThreadName, "GT", 32);
		else if (threadID == gRenderThreadID)
			StrCopy(outThreadName, "RT", 32);
		else
			sprintf_s(outThreadName, 32, "0x%x", threadID);
	}
	//////////////////////////////////////////////////////////////////////////
	const char* UGetLegibleFileName(const char* fullfilename)
	{
		const char* strFilename = StrFindRNChar(fullfilename, PATH_SEPARATOR_CHAR, 1);
		if (strFilename == nullptr) strFilename = StrFindRNChar(fullfilename, PATH_SEPARATOR_CHAR, 0);
		if (strFilename == nullptr) strFilename = fullfilename;
		return strFilename;
	}
	//////////////////////////////////////////////////////////////////////////
	const char* UEnumToStr(ELogType in)
	{
		static const char* LUTLogTypeToStr[(unsigned)ELogType::ELT_Max] = { "message", "success", "warn", "error", "fatal", "assert" };
		return LUTLogTypeToStr[(unsigned)in];
	}
	//////////////////////////////////////////////////////////////////////////
	class UAPI LogImpl : public Log
	{
	public:
		static const unsigned MAX_LISTENERS = 32;

		CriticalSection							mLock;
		TFP<void, const LogEntry&>				mListeners[MAX_LISTENERS];
		unsigned								mNumListeners = 0;
		bool									mWriteToSTDConsole = true;
		bool									mWroteToFile = false;
		TCircularQueue<LogEntry, 1024>			mLogs;


		LogImpl()
		{
			ZeroType(mListeners);
		}
		~LogImpl()
		{
			if (!mWroteToFile) WriteLogsToHTMLFile();
			mWroteToFile = true;
		}
		void WriteLogsToHTMLFile()
		{
			FILE* outFile = nullptr;
			fopen_s(&outFile, "log.html", "wb");
			fwrite(gLogHTMLBegin, 1, StrLen(gLogHTMLBegin), outFile);
			if (outFile == nullptr) return;	//failed to create log file
			
			//writing logs...
			while (LogEntry* log = mLogs.BeginPop())
			{
				char strThread[32];
				UGetLegibleThreadName(log->mThreadID, strThread);

				fprintf(outFile, "<p class=%s> [%s] [%s] [%d] [%s] &nbsp&nbsp&nbsp&nbsp&nbsp %s</p>",
					UEnumToStr(log->mType), UGetLegibleFileName(log->mFileName), log->mFunctionName, log->mLineNumber, strThread, log->mText);

				mLogs.EndPop();
			}

			fwrite(gLogHTMLEnd, 1, StrLen(gLogHTMLEnd), outFile);
			fclose(outFile);

			mWroteToFile = true;
		}
		void WriteLog(const LogEntry& log)
		{
			if(mWriteToSTDConsole)
			{
				Console::MatchColor(log.mType);

				char strThread[32];
				UGetLegibleThreadName(log.mThreadID, strThread);

				printf("[%s] [%s] [%d] [%s]  %s\n", UGetLegibleFileName(log.mFileName), log.mFunctionName, log.mLineNumber, strThread, log.mText);

			}
		}
		void CallListeners(const LogEntry& log)
		{
			for (unsigned i = 0; i < MAX_LISTENERS; i++)
				if (mListeners[i]) mListeners[i](log);
		}
		bool AddListener(TFP<void, const LogEntry&> function)
		{
			if (function)
			{
				for (unsigned i = 0; i < MAX_LISTENERS; i++)
				{
					if (mListeners[i] == nullptr)
					{
						mListeners[i] = function;
						return true;
					}
				}
			}
			return false;
		}
		void Add(ELogType type, const char* file, const char* funcName, unsigned line, const char* text)
		{
			mLock.Enter();
			
			if (mLogs.IsFull()) mLogs.Pop(LogEntry());

			LogEntry* newEntry = mLogs.Push();

			newEntry->mType = type;
			newEntry->mLineNumber = line;
			newEntry->mThreadID = Thread::ID();
			StrCopy(newEntry->mFileName, file, LogEntry::MAX_FILE_NAME);
			StrCopy(newEntry->mFunctionName, funcName, LogEntry::MAX_FUNCTION_NAME);
			StrCopy(newEntry->mText, text, LogEntry::MAX_LOG_LENGTH);

			WriteLog(*newEntry);
			CallListeners(*newEntry);
			if (type == ELT_Fatal || type == ELT_Assert)
			{				
				WriteLogsToHTMLFile();
				AppCrash();
			}
			mLock.Leave();
		}
		
	};

	//////////////////////////////////////////////////////////////////////////
	Log* Log::Get()
	{
		static TInstance<LogImpl> Ins;
		return Ins;
	}


	//////////////////////////////////////////////////////////////////////////
	void Log::AddVariadic(ELogType type, const char* file, const char* funcName, unsigned line, const char* format, ...)
	{
		char buffer[1024];
		va_list args;
		va_start(args, format);
		vsprintf_s(buffer, sizeof(buffer), format, args);
		va_end(args);

		((LogImpl*)this)->Add(type, file, funcName, line, buffer);
	}
	//////////////////////////////////////////////////////////////////////////
	void Log::AddRaw(ELogType type, const char* file, const char* funcName, unsigned line, const char* message)
	{
		((LogImpl*)this)->Add(type, file, funcName, line, message);
	}

	//////////////////////////////////////////////////////////////////////////
	bool Log::AddListener(TFP<void, const LogEntry&> function)
	{
		 return ((LogImpl*)this)->AddListener(function);
	}

};