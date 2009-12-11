#ifndef iqt_CSafeNotifier_included
#define iqt_CSafeNotifier_included


#include "iqt/iqt.h"


#include "istd/CChangeDelegator.h"
#include "istd/TChangeNotifier.h"


namespace iqt
{


class NotificationTarget;


/**
	Implementation of model changes notification between different threads.
*/
class CSafeNotifier
{
public:
	explicit CSafeNotifier(istd::IChangeable* slavePtr, int changeFlags = 0, istd::IPolymorphic* changeParamsPtr = NULL);
	~CSafeNotifier();

private:
	NotificationTarget* m_notificationTarget;
};


/**
	\internal

	Help class to realize the post-processing of istd::IChangeable::EndChanges()
*/
class NotificationTarget: protected QObject, protected istd::CChangeDelegator
{
	Q_OBJECT

	friend class CSafeNotifier;

protected:
	typedef istd::CChangeDelegator BaseClass;

	NotificationTarget(istd::IChangeable* slavePtr, int changeFlags = 0, istd::IPolymorphic* changeParamsPtr = NULL);

	void Reset();

	// reimplemented (istd::IChangeable)
	virtual void EndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr = NULL);
protected Q_SLOTS:
	void DoEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

Q_SIGNALS:
	void EmitEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

private:
	int m_changeFlags;
	istd::IPolymorphic* m_changeParamsPtr;
};


} // namespace iqt


#endif // !iqt_CSafeNotifier_included


