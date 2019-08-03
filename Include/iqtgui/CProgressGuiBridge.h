#pragma once


// Qt includes
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QAbstractButton>

// ACF includes
#include <ibase/IProgressManager.h>


namespace iqtgui
{


class CProgressGuiBridge: public QObject, virtual public ibase::IProgressManager
{
	Q_OBJECT
public:
	CProgressGuiBridge(QProgressBar* progressWidget, QAbstractButton* cancelButton = NULL);

	// reimplemented (ibase::IProgressManager)
	virtual int BeginProgressSession(
				const QByteArray& progressId,
				const QString& description,
				bool isCancelable) override;
	virtual void EndProgressSession(int sessionId);
	virtual void OnProgress(int sessionId, double currentProgress) override;
	virtual bool IsCanceled(int sessionId) const override;
	
protected Q_SLOTS:
	void OnCancelButtonClicked();

private:
	QProgressBar* m_progresBarPtr;
	QAbstractButton* m_cancelButtonPtr;
	bool m_isCanceled;
};


} // namespace iqtgui


