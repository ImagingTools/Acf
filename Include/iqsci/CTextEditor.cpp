#include "CTextEditor.h"


// Qt includes
#include <QTextEdit>
#include <QApplication>


// ACF includes
#include "iqt/CSignalBlocker.h"


// QScinitlla includes
#include <Qsci/qscilexercpp.h>


namespace iqsci
{


CTextEditor::CTextEditor(QWidget* parentWidget/* = NULL*/)
:	BaseClass(parentWidget),
	m_editorCommand("&Edit"),
	m_viewCommand("&View"),
	m_lowercaseCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_TOOLBAR),
	m_uppercaseCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_TOOLBAR),
	m_languageCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU),
	m_useIdentGuideCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_ONOFF),
	m_useFoldingCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_ONOFF),
	m_showLineNumberCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_ONOFF)
{
	m_lowercaseCommand.SetEnabled(false);
	m_uppercaseCommand.SetEnabled(false);
	m_editorCommand.InsertChild(&m_lowercaseCommand, false);
	m_editorCommand.InsertChild(&m_uppercaseCommand, false);
	m_rootCommand.InsertChild(&m_editorCommand, false);
	m_rootCommand.InsertChild(&m_viewCommand, false);
	m_rootCommand.InsertChild(&m_languageCommand, false);
	
	connect(&m_lowercaseCommand, SIGNAL(activated()), this, SLOT(OnToLowercase()));
	connect(&m_uppercaseCommand, SIGNAL(activated()), this, SLOT(OnToUppercase()));
	connect(this, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));
	connect(this, SIGNAL(selectionChanged()), this, SLOT(OnSelectionChanged()));
	
	setLexer(new QsciLexerCPP(this));
	lexer()->setDefaultFont(QFont("CourierNew", 10));

	// add view commands
	m_viewCommand.InsertChild(&m_useIdentGuideCommand, false);
	m_viewCommand.InsertChild(&m_useFoldingCommand, false);
	m_viewCommand.InsertChild(&m_showLineNumberCommand, false);

	connect(&m_useIdentGuideCommand, SIGNAL(toggled(bool)), this, SLOT(setIndentationGuides(bool)));
	connect(&m_useFoldingCommand, SIGNAL(toggled(bool)), this, SLOT(SetFoldingEnabled(bool)));
	connect(&m_showLineNumberCommand, SIGNAL(toggled(bool)), this, SLOT(SetLineNumberEnabled(bool)));

	// some additional settings:
	setBraceMatching(StrictBraceMatch);
}


void CTextEditor::OnRetranslate()
{
	m_lowercaseCommand.SetVisuals(tr("To &Lowercase"), tr("Lowercase"), tr("Convert selected block to lowercase characters"), QIcon(":/Icons/down"));
	m_uppercaseCommand.SetVisuals(tr("To &Uppercase"), tr("Uppercase"), tr("Convert selected block to uppercase characters"), QIcon(":/Icons/up"));
	m_languageCommand.SetVisuals(tr("&Language"), tr("Language"), tr("Select language for syntax highlighting"), QIcon(":/Icons/"));
	m_useIdentGuideCommand.SetVisuals(tr("Show &Indentation Guides"), tr("Show Indentation Guides"), tr("Show indentation guides"));
	m_useFoldingCommand.SetVisuals(tr("Use &Folding"), tr("Use Folding"), tr("Use folding"));
	m_showLineNumberCommand.SetVisuals(tr("Show &Line Number"), tr("Show Line Number"), tr("Show current line number"));
}


// reimplemented (idoc::ICommandsProvider)

const idoc::IHierarchicalCommand* CTextEditor::GetCommands() const
{
	return &m_rootCommand;
}


// public slots

void CTextEditor::SetFoldingEnabled(bool useFoldingEnabled)
{
	if (useFoldingEnabled){
		setFolding(CTextEditor::BoxedTreeFoldStyle); 
	}
	else{
		setFolding(CTextEditor::NoFoldStyle); 
	}
}


void CTextEditor::SetLineNumberEnabled(bool showLineNumber)
{
	setMarginLineNumbers(1, showLineNumber);
}


// protected slots

void CTextEditor::OnSelectionChanged()
{
	bool isTextSelected = hasSelectedText();

	m_lowercaseCommand.SetEnabled(isTextSelected);
	m_uppercaseCommand.SetEnabled(isTextSelected);
}


void CTextEditor::OnTextChanged()
{
	emit DataChanged();

	QString linesString = QString("%1").arg(lines());

	setMarginWidth(1, linesString);
	setMarginWidth(1, istd::Max(20, marginWidth(1)));
}


void CTextEditor::OnToLowercase()
{
	QString selectedText = this->selectedText();

	int line;
	int index;
	int line2;
	int index2;
	getSelection(&line, &index, &line2, &index2);

	removeSelectedText();

	insertAt(selectedText.toLower(), line, index);

	OnSelectionChanged();
	OnTextChanged();
}


void CTextEditor::OnToUppercase()
{
	QString selectedText = this->selectedText();

	int line;
	int index;
	int line2;
	int index2;
	getSelection(&line, &index, &line2, &index2);

	removeSelectedText();

	insertAt(selectedText.toUpper(), line, index);

	OnSelectionChanged();
	OnTextChanged();
}


}


