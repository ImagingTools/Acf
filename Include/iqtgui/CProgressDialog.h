#pragma once


// Qt includes
#include <QtWidgets/QProgressDialog>

// ACF includes
#include <ibase/IProgressManager.h>


namespace iqtgui
{


class CProgressDialog: public QProgressDialog, virtual public ibase::IProgressManager
{
public:
	typedef QProgressDialog BaseClass;

	CProgressDialog(QWidget* parentWidget = NULL);

	// reimplemented (ibase::IProgressManager)
	virtual int BeginProgressSession(
				const QByteArray& progressId,
				const QString& description,
				bool isCancelable) override;
	virtual void EndProgressSession(int sessionId);
	virtual void OnProgress(int sessionId, double currentProgress) override;
	virtual bool IsCanceled(int sessionId) const override;
};


} // namespace iqtgui


