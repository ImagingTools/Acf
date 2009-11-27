#ifndef iprm_CParamsSet_included
#define iprm_CParamsSet_included


// STL includes
#include <map>


// ACF includes
#include "istd/TChangeDelegator.h"

#include "imod/CMultiModelObserverBase.h"

#include "iprm/IParamsSet.h"


namespace iprm
{


/**
	Basic implementation of interface IParamsSet.
*/
class CParamsSet: virtual public iprm::IParamsSet
{
public:
	CParamsSet(const IParamsSet* slaveSetPtr = NULL);

	virtual bool SetEditableParameter(const std::string& id, iser::ISerializable* parameterPtr);

	// reimplemented (iprm::IParamsSet)
	virtual const iser::ISerializable* GetParameter(const std::string& id) const;
	virtual iser::ISerializable* GetEditableParameter(const std::string& id);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
	virtual I_DWORD GetMinimalVersion(int versionId = iser::IVersionInfo::UserVersionId) const;

protected:
	typedef std::map<std::string, iser::ISerializable*> ParamsMap;

	const ParamsMap& GetParamsMap() const;
	ParamsMap& GetParamsMapRef();

private:
	/**
		Class to observe the changes of the single parameters.
	*/
	class ParamsObserver: public imod::CMultiModelObserverBase
	{
	public:
		ParamsObserver(CParamsSet& parent);

	private:
		// reimplemented (imod::IObserver)
		virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CParamsSet& m_parent;
	};

	ParamsObserver m_paramsObserver;

	ParamsMap m_paramsMap;

	const IParamsSet* m_slaveSetPtr;
};


// inline methods

// protected methods

inline const CParamsSet::ParamsMap& CParamsSet::GetParamsMap() const
{
	return m_paramsMap;
}


inline CParamsSet::ParamsMap& CParamsSet::GetParamsMapRef()
{
	return m_paramsMap;
}


} // namespace iprm


#endif // !iprm_CParamsSet_included
