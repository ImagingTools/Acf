#ifndef iqt_CTimer_included
#define iqt_CTimer_included


#include <QTime>

#include "isys/ITimer.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	An timer implementation for Qt library.
*/
class CTimer: virtual public isys::ITimer
{
public:
	CTimer();

	double GetTimeTo(const CTimer& timer) const;

	/**
		Dummie implementation, provided only for compatibility with iwin::CTimer.
		Please don't use!
	*/
	unsigned long long GetNativeRepresentation() const;
	/**
		Dummie implementation, provided only for compatibility with iwin::CTimer.
		Please don't use!
	*/
	void SetNativeRepresentation(unsigned long long value);

	// reimplemented (isys::ITimer)
	virtual void Start();
	virtual double GetElapsed() const;
	virtual double GetTimeTo(const ITimer& timer) const;
	virtual void WaitTo(double time) const;
	virtual double GetTimerResolution() const;

private:
	QTime m_time;
};


} // namespace iqt


#endif // !iqt_CTimer_included
