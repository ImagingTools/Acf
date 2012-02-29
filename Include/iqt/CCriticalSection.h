#ifndef iqt_CCriticalSection_included
#define iqt_CCriticalSection_included


// Qt includes
#include <QtCore/QMutex>

// ACF includes
#include "isys/ICriticalSection.h"

#include "iqt/iqt.h"


namespace iqt
{


class CCriticalSection: public isys::ICriticalSection
{
public:
	CCriticalSection();

	// reimplemented (isys::ICriticalSection)
	virtual void Enter();
	virtual bool EnterIfFree();
	virtual void Leave();

private:
	QMutex m_section;
};


} // namespace iqt


#endif // !iqt_CCriticalSection_included


