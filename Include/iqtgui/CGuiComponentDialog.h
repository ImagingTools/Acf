/********************************************************************************
**
**	Copyright (C) 2007-2011 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the ACF Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.ilena.org, write info@imagingtools.de or contact
**	by Skype to ACF_infoline for further information about the ACF.
**
********************************************************************************/


#ifndef iqtgui_CGuiComponentDialog_included
#define iqtgui_CGuiComponentDialog_included


// Qt includes
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>

// ACF includes
#include "icomp/CComponentBase.h"

#include "iqtgui/IGuiObject.h"


namespace iqtgui
{


/**
	Class to display an ACF GUI component in a dialog.
*/
class CGuiComponentDialog: public QDialog
{
public:
	typedef QDialog BaseClass;

	/**
		Construct the component dialog.
		\param guiObjectPtr Pointer to the gui object.
		\param buttons You can define some standard buttons for this dialog. 
		Default no buttons are set and the dialog starts only with the "Close" button.
		\sa QDialogButtonBox::StandardButtons
		\param isModal controls the modality of this dialog.
		\param parentWidgetPtr - parent widget for this dialog. Default is NULL.
	*/
	CGuiComponentDialog(
				iqtgui::IGuiObject* guiObjectPtr, 
				int buttons = 0,  
				bool isModal = true, 
				QWidget* parentWidgetPtr = NULL);

	virtual ~CGuiComponentDialog();

	/**
		Returns the internal button box. 
		You can use this function to create your own connections to the signals of the button group.
	*/
	const QDialogButtonBox* GetButtonBoxPtr() const;

	/**
		Returns pointer to the connected GUI object.
	*/
	const iqtgui::IGuiObject* GetGuiPtr() const;

protected:
	// reimplemented (QWidget)
	virtual void closeEvent(QCloseEvent* eventPtr);
	virtual void keyPressEvent(QKeyEvent* eventPtr);

private:
	QDialogButtonBox* m_buttonsBox;
	iqtgui::IGuiObject* m_guiObjectPtr;
};


// public inline methods

inline const QDialogButtonBox* CGuiComponentDialog::GetButtonBoxPtr() const
{
	return m_buttonsBox;
}

	
inline const iqtgui::IGuiObject* CGuiComponentDialog::GetGuiPtr() const
{
	return m_guiObjectPtr;
}


} // namespace iqtgui


#endif // !iqtgui_CGuiComponentDialog_included


