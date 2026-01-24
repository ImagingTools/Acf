// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtWidgets/QProgressDialog>

// ACF includes
#include <ibase/CCumulatedProgressManagerBase.h>


namespace iqtgui
{


class CProgressDialog: public QProgressDialog, public ibase::CCumulatedProgressManagerBase
{
	Q_OBJECT

public:
	typedef QProgressDialog BaseClass;

	CProgressDialog(const QString& title, const QString& defaultText, QWidget* parentWidget = NULL);

protected:
	// reimplemented (ibase::CCumulatedProgressManagerBase)
	virtual void OnProgressChanged(double cumulatedValue) override;
	virtual void OnTasksChanged() override;

Q_SIGNALS:
	void TaskTextChanged(const QString& text);
	void ProgressChanged(int progress);

private:
	QString m_lastTaskText;
	int m_lastProgressValue;
};


} // namespace iqtgui


