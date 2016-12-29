#include "UCommonEd.h"

namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	class LogWidget : public QWidget
	{
		static const unsigned MAX_LOG = 256;

		QTextEdit*	mTextEdit;
		QLineEdit* mFiltter;
		QAction* mActionClear;

		QQueue<LogEntry> mLogsQueue;
		bool mIsLogDirty;
	public:

		LogWidget(QWidget* parent);
		void  FillterTextChanged(const QString&);
		void LogReceived(const LogEntry& entry);
		void FillHTML();
		void Clean();
		void Tick();
	};

	//////////////////////////////////////////////////////////////////////////
	class LogDW : public QDockWidget
	{
	public:
		LogDW(QWidget* parent);
		~LogDW();

		void Clean();

		void Tick();

		void LogReceived(const LogEntry& entry);

		static void SLogReceived(const LogEntry& newEntry);
	};

};