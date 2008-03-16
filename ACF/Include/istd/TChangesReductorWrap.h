#ifndef istd_TChangesReductorWrap_included
#define istd_TChangesReductorWrap_included


namespace istd
{


class IPolymorphic;


/**
	Help wrapper class allowing to reduce nested update notifying by counting of changes.
	If nested update is started, base class implementation of BeginChanges and EndChanges will be blocked.
*/
template <class Base>
class TChangesReductorWrap: virtual public Base
{
public:
	typedef Base BaseClass;

	TChangesReductorWrap();

	// pseudo-reimplemented (istd::IChangeable)
	virtual void BeginChanges(int changeFlags = 0, istd::IPolymorphic* changeParamsPtr = NULL);
	virtual void EndChanges(int changeFlags = 0, istd::IPolymorphic* changeParamsPtr = NULL);

private:
	int m_changeCounter;
	int m_cumulatedFlags;
};


// public methods

template <class BaseClass>
TChangesReductorWrap<BaseClass>::TChangesReductorWrap()
{
	m_changeCounter = 0;
}


// pseudo-reimplemented (istd::IChangeable)

template <class BaseClass>
void TChangesReductorWrap<BaseClass>::BeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	if (m_changeCounter++ == 0){
		BaseClass::BeginChanges(changeFlags, changeParamsPtr);
		m_cumulatedFlags = changeFlags;
	}
	else{
		m_cumulatedFlags |= changeFlags;
	}
}


template <class BaseClass>
void TChangesReductorWrap<BaseClass>::EndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	if (--m_changeCounter == 0){
		m_cumulatedFlags |= changeFlags;

		BaseClass::EndChanges(m_cumulatedFlags, changeParamsPtr);
	}
}


} // namespace istd


#endif //!istd_TChangesReductorWrap_included


