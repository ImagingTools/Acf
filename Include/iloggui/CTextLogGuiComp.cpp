#include "iloggui/CTextLogGuiComp.h"

#define PERFORMANCE_TEST


// Qt includes
#include <QtGui/QTextDocument>
#include <QtGui/QTextTable>

#ifdef PERFORMANCE_TEST
#include <QtCore/QElapsedTimer>
#endif


namespace iloggui
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CTextLogGuiComp::UpdateGui(int /*updateFlags*/)
{
#ifdef PERFORMANCE_TEST
	QElapsedTimer timer;
	timer.start();
#endif

	LogEditor->setUpdatesEnabled(false);

	LogEditor->setPlainText("");

	ilog::IMessageContainer* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		LogEditor->setUpdatesEnabled(true);

		return;
	}

	const ilog::IMessageContainer::Messages messages = objectPtr->GetMessages();
	int messagesCount = messages.count();

	QTextDocument* documentPtr = LogEditor->document();
	
	QTextCursor textCursor(documentPtr);
	textCursor.beginEditBlock();

	QTextTableFormat tableFormat;
	tableFormat.setBorder(0);
	tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_None);
	tableFormat.setCellPadding(4);
	tableFormat.setCellSpacing(0);

	QTextTable* tablePtr = textCursor.insertTable(messagesCount, 3, tableFormat);
	int tableRow = 0;

	for (int i = 0; i < messagesCount; i++, tableRow++){
		int column = 0;

		const ilog::IMessageConsumer::MessagePtr messagePtr = messages.at(i);

		int category = messagePtr->GetInformationCategory();

		QString categoryText(GetCategoryText(category));

		QTextTableCellFormat& cellFormat = i % 2 ? m_okEvenCellFormat : m_okCellFormat;

		// category
		QImage categoryIcon = GetCategoryImage(category);		
		InsertImage(tablePtr->cellAt(tableRow, column++), categoryIcon);

		// timestamp
		InsertText(tablePtr->cellAt(tableRow, column++), messagePtr->GetInformationTimeStamp().toString(), cellFormat);

		// source
		//InsertText(tablePtr->cellAt(tableRow, column++), messagePtr->GetInformationSource(), cellFormat);

		// text
		InsertText(tablePtr->cellAt(tableRow, column++), messagePtr->GetInformationDescription(), cellFormat);
	}

	textCursor.endEditBlock();

#ifdef PERFORMANCE_TEST
	textCursor.movePosition(QTextCursor::Start);
	textCursor.insertText(QString("Time: %1 ms").arg(timer.elapsed()));
#endif

	LogEditor->setUpdatesEnabled(true);
}


// reimplemented (CGuiComponentBase)

void CTextLogGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	m_okEvenCellFormat.setBackground(QColor(0xf0f0f0));
}


void CTextLogGuiComp::OnGuiDestroyed()
{
	BaseClass::OnGuiDestroyed();
}


// private members

void CTextLogGuiComp::InsertImage(QTextTableCell cell, const QImage& image)
{
	cell.firstCursorPosition().insertImage(image);
}


void CTextLogGuiComp::InsertText(QTextTableCell cell, const QString& text, const QTextTableCellFormat& cellFormat)
{
	cell.setFormat(cellFormat);

	cell.firstCursorPosition().insertText(text);
}


QImage CTextLogGuiComp::GetCategoryImage(int category) const
{
	static QImage logIcon(":/Icons/Log");
	static QImage infoIcon(":/Icons/Info.svg");
	static QImage warningIcon(":/Icons/Warning.svg");
	static QImage errorIcon(":/Icons/Error.svg");

	static bool initialized = false;
	if (!initialized){
		initialized = true;

		errorIcon = errorIcon.scaled(16,16);
		warningIcon = warningIcon.scaled(16,16);
		infoIcon = infoIcon.scaled(16,16);
		logIcon = logIcon.scaled(16,16);
	}

	switch (category){
	case istd::IInformationProvider::IC_INFO:
		return infoIcon;

	case istd::IInformationProvider::IC_WARNING:
		return warningIcon;

	case istd::IInformationProvider::IC_ERROR:
	case istd::IInformationProvider::IC_CRITICAL:
		return errorIcon;

	default:
		return logIcon;
	}
}


QString CTextLogGuiComp::GetCategoryText(int category) const
{
	switch (category){
		case istd::IInformationProvider::IC_INFO:
			return "Info";
		case istd::IInformationProvider::IC_WARNING:
			return "Warning";
		case istd::IInformationProvider::IC_ERROR:
			return "Error";
		case istd::IInformationProvider::IC_CRITICAL:
			return "Critical";
	}

	return "";
}


} // namespace iloggui


