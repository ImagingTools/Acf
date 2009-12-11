#include "iqt/CSafeNotifier.h"


namespace iqt
{


CSafeNotifier::CSafeNotifier(istd::IChangeable* slavePtr,  int changeFlags, istd::IPolymorphic* changeParamsPtr)
	:BaseClass(slavePtr),
	m_notifier(NULL, changeFlags, changeParamsPtr)
{
	connect(
				this,
				SIGNAL(EmitBeginChanges(int, istd::IPolymorphic*)),
				this, 
				SLOT(DoBeginChanges(int, istd::IPolymorphic*)),
				Qt::QueuedConnection);
	connect(
				this,
				SIGNAL(EmitEndChanges(int, istd::IPolymorphic*)),
				this, 
				SLOT(DoEndChanges(int, istd::IPolymorphic*)),
				Qt::QueuedConnection);

	m_notifier.SetPtr(this);
}


// reimplemented (istd::IChangeable)

void CSafeNotifier::BeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	Q_EMIT EmitBeginChanges(changeFlags, changeParamsPtr);
}


void CSafeNotifier::EndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	Q_EMIT EmitEndChanges(changeFlags, changeParamsPtr);
}


// protected slots:
void CSafeNotifier::DoBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::BeginChanges(changeFlags, changeParamsPtr);
}

void CSafeNotifier::DoEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::EndChanges(changeFlags, changeParamsPtr);
}


} // namespace iqt


