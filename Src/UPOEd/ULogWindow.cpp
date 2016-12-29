#include "ULogWindow.h"

namespace UPOEd
{
	const char* gLogHTMLBegin =
		"<!DOCTYPE html><html><head><style>"
		"h1, h2{ color: white;	margin: 0; }"
// 		"p{ color: red; }"
// 		"p.s0{ color: white; }"
// 		"p.s1{ color: #66ff33; }"
// 		"p.s2{ color: yellow; }"
// 		"p.s3{ color: red; }"
// 		"p.s4{ color: red; }"
// 		"p.s5{ color: red; }"
 		"body{ background-color : black; }"
		"</style></head><body>"
		"<h1>UPOEngine log file</h1>"
		"<h2>[File Name] [Function Name] [LineNumber] [ThreadID] log</h2>"
		"<p></p>";

	const char* gLogHTMLEnd =
		"</body></html>";


	LogDW* gLogWindow = nullptr;
	void ULogReceived(const LogEntry& newEntry)
	{
		if (gLogWindow) gLogWindow->LogReceived(newEntry);
	}

	//////////////////////////////////////////////////////////////////////////
	LogDW::LogDW(QWidget* parent) : QDockWidget(parent)
	{
		UASSERT(gLogWindow == nullptr);	//only one instance should be created

		gLogWindow = this;

		Log::Get()->AddListener(ULogReceived);

		this->setWidget(new LogWidget(nullptr));

		this->setAllowedAreas(Qt::DockWidgetArea::TopDockWidgetArea | Qt::DockWidgetArea::BottomDockWidgetArea);
		this->setWindowTitle("Log");
		this->setContentsMargins(0, 0, 0, 0);
	}
	//////////////////////////////////////////////////////////////////////////
	LogDW::~LogDW()
	{
		gLogWindow = nullptr;
	}

	void LogDW::Clean()
	{
		((LogWidget*)this->widget())->Clean();
	}

	void LogDW::Tick()
	{
		((LogWidget*)this->widget())->Tick();
	}

	//////////////////////////////////////////////////////////////////////////
	void LogDW::LogReceived(const LogEntry& newEntry)
	{
		((LogWidget*)this->widget())->LogReceived(newEntry);
	}
	//////////////////////////////////////////////////////////////////////////
	void LogDW::SLogReceived(const LogEntry& newEntry)
	{
		if (gLogWindow) gLogWindow->LogReceived(newEntry);
	}
	//////////////////////////////////////////////////////////////////////////
	LogWidget::LogWidget(QWidget* parent) : QWidget(parent)
	{
		// 			mListFileName = new QListWidget();
		// 			mListFunction = new QListWidget();
		// 			mListLineNumber = new QListWidget();
		// 			mListThreadID = new QListWidget();
		// 			mListLog = new QListWidget();
		// 
		// 			QSplitter* splt = new QSplitter(this);
		// 			splt->addWidget(mListFileName);
		// 			splt->addWidget(mListFunction);
		// 			splt->addWidget(mListLineNumber);
		// 			splt->addWidget(mListThreadID);
		// 			splt->addWidget(mListLog);
		// 			splt->setOrientation(Qt::Orientation::Horizontal);

		setLayout(new QVBoxLayout);
		layout()->setContentsMargins(8, 0, 8, 0);
		mFiltter = new QLineEdit(this);
		mFiltter->setClearButtonEnabled(true);

		layout()->addWidget(mFiltter);
		connect(mFiltter, &QLineEdit::textChanged, this, &LogWidget::FillterTextChanged);

		mTextEdit = new QTextEdit(this);
		layout()->addWidget(mTextEdit);
		mTextEdit->setReadOnly(true);


		{
			mTextEdit->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

			////////////clear action
			mActionClear = new QAction("Clear", this);
			connect(mActionClear, &QAction::triggered, this, [&](bool) {
				Clean();
			});

			//adding clear to context menu
			{
				connect(mTextEdit, &QTextEdit::customContextMenuRequested, this, [&](const QPoint p)
				{
					QMenu* menu = mTextEdit->createStandardContextMenu(p);
					menu->addAction(mActionClear);
					menu->exec(mTextEdit->mapToGlobal(p));
					delete menu;
				});

				mTextEdit->addAction(mActionClear);
			}
		}


		FillHTML();
	}

	void LogWidget::FillterTextChanged(const QString&)
	{
		FillHTML();
	}
	QColor GetLogTypeQColor(ELogType type)
	{
		switch (type)
		{
		case UPO::ELT_Message: return QColor(255, 255, 255);
		case UPO::ELT_Success: return QColor(0, 255, 0);
		case UPO::ELT_Warn: return QColor(255, 255, 0);
		case UPO::ELT_Error:
		case UPO::ELT_Fatal:
		case UPO::ELT_Assert:
			return QColor(255, 0, 0);
		}
		return QColor(255, 255, 255);
	}
	void LogWidget::LogReceived(const LogEntry& entry)
	{
		if (mLogsQueue.count() > MAX_LOG)
			mLogsQueue.dequeue();
		mLogsQueue.enqueue(entry);
		mIsLogDirty = true;
	}
	void LogWidget::Tick()
	{
		if(mIsLogDirty)
		{
			mIsLogDirty = false;
			FillHTML();

			//set scroll bottom
			mTextEdit->verticalScrollBar()->setValue(mTextEdit->verticalScrollBar()->maximum());
		}

	}
	void LogWidget::FillHTML()
	{

		QString htmlResult;
		
		htmlResult.reserve(1024 * 10);

		htmlResult += gLogHTMLBegin;




		auto iter = mLogsQueue.cbegin();
		while (iter != mLogsQueue.cend())
		{
			char strThreadID[32];
			sprintf_s(strThreadID, sizeof(strThreadID), "0x%x", iter->mThreadID);
			const char* strThread = strThreadID;
			if (iter->mThreadID == gGameThreadID) strThread = "GT";
			if (iter->mThreadID == gRenderThreadID) strThread = "RT";

			const char* strFilename = StrFindRNChar(iter->mFileName, PATH_SEPARATOR_CHAR, 1);
			if (strFilename == nullptr) strFilename = StrFindRNChar(iter->mFileName, PATH_SEPARATOR_CHAR, 0);
			if (strFilename == nullptr) strFilename = iter->mFileName;

			//QString line = QString::asprintf("<p  style=\"color: #ff00ff;\"> [%s] [%s] [%d] [%s]     %s</p>",
			
			char* strSyle = "color: red;";
			if (iter->mType == ELogType::ELT_Message) strSyle = "color: white;";
			else if (iter->mType == ELogType::ELT_Success) strSyle = "color: #66ff33;";
			else if (iter->mType == ELogType::ELT_Warn) strSyle = "color: yellow";


			QString line = QString::asprintf("<p  style=\"%s\"> [%s] [%s] [%d] [%s] \t\t    %s</p>",
				 strSyle, strFilename, iter->mFunctionName, iter->mLineNumber, strThreadID, iter->mText);
			
			if(FilterCheck(mFiltter->text(), line))
				htmlResult += line;

			iter++;
		}

		htmlResult += gLogHTMLEnd;

		mTextEdit->setHtml(htmlResult);
	}

	void LogWidget::Clean()
	{
		mLogsQueue.clear();
		FillHTML();
	}

};