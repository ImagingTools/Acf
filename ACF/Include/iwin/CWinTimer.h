#ifndef iwin_CWinTimer_included
#define iwin_CWinTimer_included


#include "iwin/iwin.h"

#include "isys/ITimer.h"


namespace iwin
{


/**
	\ingroup iwin

	An timer implementation for the Windows plattform
*/
class CWinTimer: public isys::ITimer
{
public:
	CWinTimer();

	bool IsVaild() const;

	// reimplemented (isys::ITimer)
	virtual void Start();
	virtual double GetElapsed() const;
	virtual double GetTimerResolution() const;

protected:
	long long m_startCounter;
	long long m_timerFrequence;
	bool m_isValid;
};


} // namespace iwin


#endif // !iwin_CWinTimer_included
