#pragma once

#include "UCommon.h"

namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	class ColorSelectionDialog : public QColorDialog
	{


		static ColorSelectionDialog* Active;
		static QRect LastGemetry;

		QList<QMetaObject::Connection> mActiveConnections;


	public:
		ColorSelectionDialog(QWidget* parent = nullptr) : QColorDialog(parent)
		{
			setFocusPolicy(Qt::FocusPolicy::ClickFocus);
			setOption(QColorDialog::ShowAlphaChannel, true);
		}

		//opens the color selection dialog right now, will be closed whenever it lose focus
		static ColorSelectionDialog* OpenNow(QColor color);
		//void (void)
		template <typename T, typename Lambda> static void OnRejected(T* receiver, Lambda receiverFunc)
		{
			Active->mActiveConnections << connect(Active, &QColorDialog::rejected, receiver, receiverFunc);
		}
		//void (void)
		template <typename T, typename Lambda> static void OnAccepted(T* receiver, Lambda receiverFunc)
		{
			Active->mActiveConnections << connect(Active, &QColorDialog::accepted, receiver, receiverFunc);
		}
		//void (const QColor&)
		template <typename T, typename Lambda> static void OnColorChanged(T* receiver, Lambda receiverFunc)
		{
			Active->mActiveConnections << connect(Active, &QColorDialog::currentColorChanged, receiver, receiverFunc);
		}

	private:
		void CloseConnections()
		{
			for (auto item : mActiveConnections) QObject::disconnect(item);
		}
		//this is called before reject and accept
		virtual void focusOutEvent(QFocusEvent *event) override
		{
			LastGemetry = this->geometry();
			QColorDialog::focusOutEvent(event);
			//close();

		}
		virtual void accept() override
		{
			QColorDialog::accept();
			CloseConnections();
		}
		virtual void reject() override
		{
			QColorDialog::reject();
			CloseConnections();
		}

	};
};
