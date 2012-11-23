#ifndef istd_CEventBasedNotifier_included
#define istd_CEventBasedNotifier_included


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TOptDelPtr.h"


namespace istd
{


class NotificationTarget;


/**
	Implementation of model changes notification between different threads.
*/
class CEventBasedNotifier
{
public:
	explicit CEventBasedNotifier(istd::IChangeable* slavePtr, int changeFlags = 0, istd::IPolymorphic* changeParamsPtr = NULL);
	~CEventBasedNotifier();

private:
	NotificationTarget* m_notificationTarget;
};


/**
	\internal

	Help class to realize the post-processing of istd::IChangeable::EndChanges()
*/
class NotificationTarget: protected QObject
{
	Q_OBJECT

	friend class CEventBasedNotifier;

protected:
	NotificationTarget(istd::IChangeable* slavePtr, int changeFlags = 0, istd::IPolymorphic* changeParamsPtr = NULL);

	void Reset();

protected Q_SLOTS:
	void DoBeginChanges();
	void DoEndChanges();

Q_SIGNALS:
	void EmitBeginChanges();
	void EmitEndChanges();

private:
	istd::IChangeable* m_slavePtr;
	int m_changeFlags;
	istd::TOptDelPtr<istd::IPolymorphic> m_changeParamsPtr;

	/**
		Flag to inidicate, that BeginChange notification was sent.
	*/
	bool m_isBeginPending;
};


} // namespace istd


#endif // !istd_CEventBasedNotifier_included


