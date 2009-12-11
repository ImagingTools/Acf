#ifndef iqt_CSafeNotifier_included
#define iqt_CSafeNotifier_included


#include "iqt/iqt.h"


#include "istd/CChangeDelegator.h"
#include "istd/TChangeNotifier.h"


namespace iqt
{


/**
	Implementation of model changes notification between different threads.
*/
class CSafeNotifier: public QObject, public istd::CChangeDelegator
{
	Q_OBJECT
public:
	typedef istd::CChangeDelegator BaseClass;

	explicit CSafeNotifier(istd::IChangeable* slavePtr, int changeFlags = 0, istd::IPolymorphic* changeParamsPtr = NULL);

	// reimplemented (istd::IChangeable)
	virtual void BeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr = NULL);
	virtual void EndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr = NULL);

protected Q_SLOTS:
	void DoBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);
	void DoEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

signals:
	void EmitBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);
	void EmitEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

private:
	istd::CChangeNotifier m_notifier;
};


} // namespace iqt


#endif // !iqt_CSafeNotifier_included


