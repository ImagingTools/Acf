#ifndef istd_TChangeNotifier_included
#define istd_TChangeNotifier_included


#include "istd/istd.h"


namespace istd
{


/**
	Help class which provides the automatic update mechanism of the model.
*/
template <class Changeable>
class TChangeNotifier   
{
public:
	explicit TChangeNotifier(Changeable* changeablePtr, int changeFlags = 0);
	virtual  ~TChangeNotifier();

	void Reset();
	bool IsValid() const;
	const Changeable* operator->() const;
	Changeable* operator->();
	operator Changeable*() const;

private:
	Changeable* m_changeablePtr;
	int m_changeFlags;
};


template <class Changeable>
TChangeNotifier<Changeable>::TChangeNotifier(Changeable* changeablePtr, int changeFlags)
	:m_changeablePtr(changeablePtr),
	m_changeFlags(changeFlags)
{
	if (m_changeablePtr != NULL){
		m_changeablePtr->BeginChanges(changeFlags);
	}
}


template <class Changeable>
TChangeNotifier<Changeable>::~TChangeNotifier()
{
	Reset();
}



template <class Changeable>
inline void TChangeNotifier<Changeable>::Reset()
{
	if (IsValid()){
		m_changeablePtr->EndChanges(m_changeFlags);
	}

	m_changeablePtr = NULL;
}


template <class Changeable>
inline bool TChangeNotifier<Changeable>::IsValid() const
{
	return (m_changeablePtr != NULL);
}


template <class Changeable>
inline const Changeable* TChangeNotifier<Changeable>::operator->() const
{
	return m_changeablePtr;
}


template <class Changeable>
inline Changeable* TChangeNotifier<Changeable>::operator->()
{
	return m_changeablePtr;
}


template <class Changeable>
inline TChangeNotifier<Changeable>::operator Changeable*() const
{
	return m_changeablePtr;
}


} // namespace istd


#endif // !istd_TChangeNotifier_included

