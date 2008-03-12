#ifndef imath_CFulcrumFunctionBase_included
#define imath_CFulcrumFunctionBase_included


#include "istd/TChangesReductorWrap.h"

#include "iser/ISerializable.h"

#include "imath/TIMathFunction.h"
#include "imath/CVector2d.h"


namespace imath
{


class CFulcrumFunctionBase: public istd::TChangesReductorWrap<iser::ISerializable>, public IDoubleFunction
{
public:
	typedef istd::TChangesReductorWrap<iser::ISerializable> BaseClass;

	/**
		Removes all fulcrum points.
	*/
	void Reset();

	/**
		Get number of fulcrum points used in this function.
	*/
	int GetFulcrumsCount() const;

	/**
		Set number of fulcrum points used in this function.
		Please note, that fulcrums are automatically sorted by its X values in update routine.
		If you want to avoid this process, block update with istd::TChangeNotifier.
	*/
	void SetFulcrumsCount(int count);

	/**
		Get fulcrum point.
	*/
	const CVector2d& GetFulcrum(int index) const;

	/**
		Set single fulcrum point.
		Please note, that fulcrums are automatically sorted by its X values in update routine.
		If you want to avoid this process, block update with istd::TChangeNotifier.
	*/
	void SetFulcrum(int index, const CVector2d& fulcrum);

	/**
		Remove single fulcrum point.
		Please note, that fulcrums are automatically sorted by its X values in update routine.
		If you want to avoid this process, block update with istd::TChangeNotifier.
	*/
	void RemoveFulcrum(int index);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (istd::IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

	/**
		Sort fulcrums in this collection.
	*/
	virtual void SortFulcrums();

	// static methods
	static bool SortFulcrumFunc(const CVector2d& first, const CVector2d& second);

private:
	typedef ::std::vector<CVector2d> Fulcrums;

	Fulcrums m_fulcrums;
};


// inline protected methods

// static methods

inline bool CFulcrumFunctionBase::SortFulcrumFunc(const CVector2d& first, const CVector2d& second)
{
	return first.GetX() < second.GetX();
}


} // namespace imath


#endif // !imath_CFulcrumFunctionBase_included


