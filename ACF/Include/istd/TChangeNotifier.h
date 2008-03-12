#ifndef istd_TChangeNotifier_included
#define istd_TChangeNotifier_included


#include "istd/istd.h"

#include "istd/IChangeable.h"


namespace istd
{


/**
	Help class which provides the automatic update mechanism of the model.
*/
template <class Changeable>
class TChangeNotifier   
{
public:
	explicit TChangeNotifier(Changeable* changeablePtr, int changeFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);
	virtual  ~TChangeNotifier();

	void SetChangeable(Changeable* changeablePtr);
	void Reset();
	bool IsValid() const;
	const Changeable* operator->() const;
	Changeable* operator->();
	operator Changeable*() const;

private:
	Changeable* m_changeablePtr;
	int m_changeFlags;
	istd::IPolymorphic* m_updateParamsPtr;
};


template <class Changeable>
TChangeNotifier<Changeable>::TChangeNotifier(Changeable* changeablePtr, int changeFlags, istd::IPolymorphic* updateParamsPtr)
:	m_changeablePtr(changeablePtr),
	m_changeFlags(changeFlags),
	m_updateParamsPtr(updateParamsPtr)
{
	if (m_changeablePtr != NULL){
		m_changeablePtr->BeginChanges(m_changeFlags, m_updateParamsPtr);
	}
}


template <class Changeable>
TChangeNotifier<Changeable>::~TChangeNotifier()
{
	Reset();
}



template <class Changeable>
void TChangeNotifier<Changeable>::SetChangeable(Changeable* changeablePtr)
{
	Reset();

	m_changeablePtr = changeablePtr;

	if (m_changeablePtr != NULL){
		m_changeablePtr->BeginChanges(m_changeFlags, m_updateParamsPtr);
	}
}


template <class Changeable>
inline void TChangeNotifier<Changeable>::Reset()
{
	if (m_changeablePtr != NULL){
		m_changeablePtr->EndChanges(m_changeFlags, m_updateParamsPtr);

		m_changeablePtr = NULL;
	}
}


template <class Changeable>
inline bool TChangeNotifier<Changeable>::IsValid() const
{
	return (m_changeablePtr != NULL);
}


template <class Changeable>
inline const Changeable* TChangeNotifier<Changeable>::operator->() const
{
	I_ASSERT(m_changeablePtr != NULL);

	return m_changeablePtr;
}


template <class Changeable>
inline Changeable* TChangeNotifier<Changeable>::operator->()
{
	I_ASSERT(m_changeablePtr != NULL);

	return m_changeablePtr;
}


template <class Changeable>
inline TChangeNotifier<Changeable>::operator Changeable*() const
{
	I_ASSERT(m_changeablePtr != NULL);

	return m_changeablePtr;
}


typedef TChangeNotifier<IChangeable> CChangeNotifier;


} // namespace istd


#endif // !istd_TChangeNotifier_included

