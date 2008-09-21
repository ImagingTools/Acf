#include "iqt/CLogGuiComp.h"


#include <QHeaderView>
#include <QDateTime>


namespace iqt
{


// public methods

CLogGuiComp::CLogGuiComp()
{
	m_categoryNameMap[ibase::IMessage::MC_INFO] = tr("Info");
	m_categoryNameMap[ibase::IMessage::MC_WARNING] = tr("Warning");
	m_categoryNameMap[ibase::IMessage::MC_ERROR] = tr("Error");
	m_categoryNameMap[ibase::IMessage::MC_CRITICAL] = tr("Critical");

	connect(this, SIGNAL(EmitAddMessage(ibase::IMessage*)), this, SLOT(OnAddMessage(ibase::IMessage*)), Qt::QueuedConnection);
	connect(this, SIGNAL(EmitRemoveMessage(ibase::IMessage*)), this, SLOT(OnRemoveMessage(ibase::IMessage*)), Qt::QueuedConnection);
}


// protected methods
	
// reimplemented (CGuiComponentBase)

void CLogGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();


	LogView->header()->setStretchLastSection(true);

	ExportButton->setVisible(m_fileLoaderCompPtr.IsValid());

	if (m_maxMessageCountAttrPtr.IsValid()){
		SetMaxMessageCount(m_maxMessageCountAttrPtr->GetValue());
	}
}


// reimplemented (istd::IChangeable)

void CLogGuiComp::OnBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	if (changeFlags & ibase::IMessageContainer::Reset){
		LogView->clear();
	}

	if (changeFlags & ibase::IMessageContainer::MessageRemoved){
		ibase::IMessage* messagePtr = dynamic_cast<ibase::IMessage*>(changeParamsPtr);
		if (messagePtr != NULL){
			emit EmitRemoveMessage(messagePtr);
		}
	}

	BaseClass2::OnBeginChanges(changeFlags, changeParamsPtr);	
}


void CLogGuiComp::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	if (changeFlags & ibase::IMessageContainer::MessageAdded){
		ibase::IMessage* messagePtr = dynamic_cast<ibase::IMessage*>(changeParamsPtr);
		if (messagePtr != NULL){
			emit EmitAddMessage(messagePtr);
		}
	}

	BaseClass2::OnEndChanges(changeFlags, changeParamsPtr);
}


// protected slots

void CLogGuiComp::OnAddMessage(ibase::IMessage* messagePtr)
{	
	I_ASSERT(messagePtr != NULL);

	CMessageItem* messageItemPtr = new CMessageItem();
	messageItemPtr->messagePtr = messagePtr;

	QDateTime dateTime;
	dateTime = QDateTime::fromTime_t(messagePtr->GetTimeStamp().ToCTime());

	QString date = dateTime.toString();
	messageItemPtr->setText(TimeColumn, date);
	messageItemPtr->setText(MessageColumn, iqt::GetQString(messagePtr->GetText()));
	messageItemPtr->setText(SourceColumn, iqt::GetQString(messagePtr->GetSource()));

	messageItemPtr->setToolTip(TimeColumn, iqt::GetQString(messagePtr->GetText()));
	messageItemPtr->setToolTip(MessageColumn, iqt::GetQString(messagePtr->GetText()));
	messageItemPtr->setToolTip(SourceColumn, iqt::GetQString(messagePtr->GetText()));

	QColor messageColor = GetMessageColor(*messagePtr);

	messageItemPtr->setBackgroundColor(TimeColumn, messageColor);
	messageItemPtr->setBackgroundColor(SourceColumn, messageColor);
	messageItemPtr->setBackgroundColor(MessageColumn, messageColor);

	LogView->addTopLevelItem(messageItemPtr);
	if (NeedToBeHidden(*messagePtr)){
		messageItemPtr->setHidden(true);
	}
}


void CLogGuiComp::OnRemoveMessage(ibase::IMessage* messagePtr)
{
	I_ASSERT(messagePtr != NULL);

	for (int itemIndex = 0; itemIndex < LogView->topLevelItemCount(); itemIndex++){
		CMessageItem* messageItemPtr = dynamic_cast<CMessageItem*>(LogView->topLevelItem(itemIndex));
		I_ASSERT(messageItemPtr != NULL);

		if (messageItemPtr->messagePtr == messagePtr){
			LogView->takeTopLevelItem(itemIndex);

			return;
		}
	}
}


void CLogGuiComp::on_ClearButton_clicked()
{
	ClearMessages();
}


void CLogGuiComp::on_ExportButton_clicked()
{
	if (m_fileLoaderCompPtr.IsValid()){
		m_fileLoaderCompPtr->SaveToFile(*this, istd::CString());
	}
}


void CLogGuiComp::on_CategorySlider_valueChanged(int category)
{
	for (int itemIndex = 0; itemIndex < LogView->topLevelItemCount(); itemIndex++){
		CMessageItem* messageItemPtr = dynamic_cast<CMessageItem*>(LogView->topLevelItem(itemIndex));
		I_ASSERT(messageItemPtr != NULL);

		if (NeedToBeHidden(*messageItemPtr->messagePtr)){
			messageItemPtr->setHidden(true);
		}
		else{
			messageItemPtr->setHidden(false);
		}
	}

	CategoryLabel->setText(m_categoryNameMap[category]);
}


// private methods

QColor CLogGuiComp::GetMessageColor(const ibase::IMessage& message) const
{
	QColor messageColor(0,0,0,0);

	int category = message.GetCategory();

	switch(category){
		case ibase::IMessage::MC_WARNING:
			messageColor = QColor(235, 235, 0);
			break;

		case ibase::IMessage::MC_ERROR:
			messageColor = QColor(255, 0, 255, 128);
			break;

		case ibase::IMessage::MC_CRITICAL:
			messageColor = QColor(255, 0, 0);
			break;
	}

	return messageColor;
}


bool CLogGuiComp::NeedToBeHidden(const ibase::IMessage& message) const
{
	int currentCategory = CategorySlider->value();

	int itemCategory = message.GetCategory();
	if (itemCategory < currentCategory){
		return true;
	}

	return false;
}


} // namespace iqt


