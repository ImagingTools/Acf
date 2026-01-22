// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// Qt includes
#include <QtGui/QTextTableCell>

// ACF includes
#include <ilog/IMessageContainer.h>
#include <ilog/CMessage.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <GeneratedFiles/iloggui/ui_CTextLogGuiComp.h>


namespace iloggui
{


/**
	Text-based log viewer with rich formatting.
	
	CTextLogGuiComp provides a read-only, rich-text display of log messages using
	QTextEdit. It observes a message container and updates its display automatically
	when messages are added.
	
	The component displays messages in a formatted table with icons, timestamps,
	sources, and message text. It provides filtering controls for severity, source,
	and message content.
	
	This component is simpler than CLogGuiComp and is best suited for read-only
	log displays where advanced features like export and commands aren't needed.
	
	For detailed documentation including configuration and usage examples, see
	the \ref page_iloggui "iloggui library documentation".
	
	\ingroup iloggui
	
	\par Quick Example
	\code{.cpp}
	// Create log container
	istd::TSharedInterfacePtr<ilog::CLogComp> logContainer(
	    new ilog::CLogComp);
	
	// Create text log viewer
	istd::TSharedInterfacePtr<iloggui::CTextLogGuiComp> textLog(
	    new iloggui::CTextLogGuiComp);
	
	// Configure to observe container
	// In .acc: Model -> logContainer
	
	// Get widget for display
	QWidget* widget = textLog->GetWidget();
	layout->addWidget(widget);
	
	// Messages added to logContainer appear automatically
	\endcode
	
	\see \ref page_iloggui, ilog::IMessageContainer, iqtgui::TDesignerGuiObserverCompBase
*/
class CTextLogGuiComp:
	public iqtgui::TDesignerGuiObserverCompBase<
				Ui::CTextLogGuiComp, ilog::IMessageContainer>
{
	Q_OBJECT

public:
	/// Base class typedef
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CTextLogGuiComp, ilog::IMessageContainer> BaseClass;

	I_BEGIN_COMPONENT(CTextLogGuiComp);
		I_ASSIGN_MULTI_0(m_ignoreIdsListAttrPtr, "IgnoreIdsList", "List if message IDs to be ignored", false);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiRetranslate() override;

private Q_SLOTS:
	void DoFilter();

private:
	void UpdateFilters();
	void GenerateDocument(int severityFilter, const QString& sourceFilter, const QString& textFilter);
	void InsertImage(QTextTableCell cell, const QImage& image);
	void InsertText(QTextTableCell cell, const QString& text, const QTextTableCellFormat& cellFormat);
	QIcon GetCategoryIcon(int category) const;
	QImage GetCategoryImage(int category) const;
	QString GetCategoryText(int category) const;

	QTextTableFormat m_tableFormat;
	QTextTableCellFormat m_okCellFormat;

	/**
		List of message IDs to be ignored.
	*/
	I_MULTIATTR(int, m_ignoreIdsListAttrPtr);
};


} // namespace iloggui


