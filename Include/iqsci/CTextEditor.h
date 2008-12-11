#ifndef iqsci_CTextEditor_included
#define iqsci_CTextEditor_included


// QScinitlla includes
#include <Qsci/QsciScintilla.h>


// ACF includes
#include "idoc/ICommandsProvider.h"

#include "iqtgui/CHierarchicalCommand.h"


namespace iqsci
{


class CTextEditor: public QsciScintilla, virtual public idoc::ICommandsProvider
{
	Q_OBJECT

public:
	typedef QsciScintilla BaseClass;

	CTextEditor(QWidget* parentWidget = NULL);

	virtual void OnRetranslate();

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

public slots:
	virtual void SetFoldingEnabled(bool useFoldingEnabled);
	virtual void SetLineNumberEnabled(bool useFoldingEnabled);

protected slots:
	virtual void OnSelectionChanged();
	virtual void OnTextChanged();
	virtual void OnToLowercase();
	virtual void OnToUppercase();

signals:
	void DataChanged();

private:
	iqtgui::CHierarchicalCommand m_rootCommand;
	iqtgui::CHierarchicalCommand m_editorCommand;
	iqtgui::CHierarchicalCommand m_viewCommand;
	iqtgui::CHierarchicalCommand m_languageCommand;
	iqtgui::CHierarchicalCommand m_lowercaseCommand;
	iqtgui::CHierarchicalCommand m_uppercaseCommand;

	// view commands
	iqtgui::CHierarchicalCommand m_useIdentGuideCommand;
	iqtgui::CHierarchicalCommand m_useFoldingCommand;
	iqtgui::CHierarchicalCommand m_showLineNumberCommand;

};


} // namespace iqsci



#endif // !iqsci_CTextEditor_included




