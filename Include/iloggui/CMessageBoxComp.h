// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QVector>
#include <QtGui/QIcon>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QMessageBox>
#endif

// ACF includes
#include <icomp/CComponentBase.h>
#include <ilog/IMessageConsumer.h>
#include <ilog/CMessage.h>


namespace iloggui
{


/**
	Component for displaying log messages in modal Qt message boxes.
	
	CMessageBoxComp provides a simple way to display important messages (especially
	errors and warnings) to users in standard Qt message box dialogs. It implements
	IMessageConsumer to receive messages and displays them in modal dialogs with
	appropriate icons.
	
	Messages are queued and displayed one at a time. If multiple messages are added
	before the dialog is shown, they're batched into a single dialog with a detailed
	text area.
	
	\ingroup iloggui
	
	\par Features
	- Modal message box display
	- Severity-appropriate icons (info, warning, critical)
	- Thread-safe message queuing with QMutex
	- Batch multiple messages into one dialog
	- Signal/slot based asynchronous display
	- Automatic message formatting
	
	\par Message Icons
	- IC_INFO, IC_NONE: Information icon (blue "i")
	- IC_WARNING: Warning icon (yellow triangle)
	- IC_ERROR, IC_CRITICAL: Critical icon (red "X")
	
	\par Usage Example
	\code{.cpp}
	// Create message box component
	istd::TSharedInterfacePtr<iloggui::CMessageBoxComp> msgBox(
	    new iloggui::CMessageBoxComp);
	
	// Send error message (displays immediately)
	msgBox->AddMessage(istd::TSharedInterfacePtr<ilog::CMessage>(
	    new ilog::CMessage(
	        istd::IInformationProvider::IC_CRITICAL,
	        5001,
	        "Database connection failed",
	        "DatabaseManager"
	    )
	));
	
	// Dialog blocks until user clicks OK
	\endcode
	
	\par Component Configuration
	\code{.xml}
	<Component Id="ErrorDialog" Class="iloggui::CMessageBoxComp">
	    <!-- No additional configuration needed -->
	</Component>
	\endcode
	
	\par Integration with Logging
	\code{.cpp}
	// Create main log
	istd::TSharedInterfacePtr<ilog::CLogComp> mainLog(
	    new ilog::CLogComp);
	
	// Create message box for errors
	istd::TSharedInterfacePtr<iloggui::CMessageBoxComp> errorBox(
	    new iloggui::CMessageBoxComp);
	
	// Create router to send only errors to message box
	istd::TSharedInterfacePtr<ilog::CLogRouterComp> router(
	    new ilog::CLogRouterComp);
	// Configure:
	//   InputMessageContainer -> mainLog
	//   OutputMessageConsumer -> errorBox
	//   MinimalCategory -> IC_ERROR
	
	// All messages go to log, but only errors show in dialogs
	\endcode
	
	\warning Message boxes are modal and block the application. Use sparingly
	         for critical messages only, not for general logging.
	
	\see ilog::IMessageConsumer, ilog::CMessage, QMessageBox
*/
class CMessageBoxComp:
			public QObject,
			public icomp::CComponentBase,
			virtual public ilog::IMessageConsumer
{
	Q_OBJECT

public:
	/// Base class typedef
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CMessageBoxComp);
		I_REGISTER_INTERFACE(ilog::IMessageConsumer);
	I_END_COMPONENT;

	CMessageBoxComp();

	// reimplemented (ilog::IMessageConsumer)
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const override;
	virtual void AddMessage(const MessagePtr& messagePtr) override;

protected:
	/**
		Get icons corresponding to specified information category.
	*/
	QIcon GetCategoryIcon(int category) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	void CreateMessageText(QString& messageText, QString& detailedText, QIcon& statusIcon) const;

protected Q_SLOTS:
	virtual void OnAddMessage();

Q_SIGNALS:
	void EmitAddMessage();

private:
	QMessageBox m_messageBox;
	QMutex m_messageQueueMutex;

	QVector<ilog::CMessage> m_messageQueue;
};


} // namespace iloggui




