#ifndef iprm_TParamsPtr_included
#define iprm_TParamsPtr_included


// Qt includes
#include <QtCore/QStringList>

// ACF includes
#include "istd/TPointerBase.h"
#include "icomp/TAttribute.h"
#include "icomp/TAttributeMember.h"
#include "icomp/TReferenceMember.h"
#include "iprm/IParamsSet.h"


namespace iprm
{


/**
	Help pointer wrapper for management of a parameter from the parameter set.
*/
template <class ParameterInterace>
class TParamsPtr: public istd::TPointerBase<const ParameterInterace>
{
public:
	typedef istd::TPointerBase<const ParameterInterace> BaseClass;

	TParamsPtr(const ParameterInterace* ptr = NULL);

	/**
		Construct and initialize the pointer with the given parameter set and parameter ID.
		\param	parameterSetPtr	parameter set, parameter will be taken from this set.
		\param	parameterId		ID of parameter in the set.
		\param	isObligatory	indicate, that this parameter is obligatory.
	*/
	TParamsPtr(const IParamsSet* parameterSetPtr, const QByteArray& parameterId, bool isObligatory = true);

	/**
		Construct and initialize the pointer with the component parameters.
		\param	parameterSetPtr			parameter set, parameter will be taken from this set.
		\param	parameterIdAttribute	component attribute storing ID of parameter in the set.
		\param	defaultRef				component reference used if parameter is not found in the set.
		\param	isObligatory			indicate, that this parameter is obligatory.
	*/
	TParamsPtr(	const IParamsSet* parameterSetPtr,
				const icomp::TAttributeMember<icomp::CIdAttribute>& parameterIdAttribute,
				const icomp::TReferenceMember<ParameterInterace>& defaultRef,
				bool isObligatory = true);

	/**
		Initialize the pointer with the given parameter set and parameter ID.
		\param	parameterSetPtr	parameter set, parameter will be taken from this set.
		\param	parameterId		ID of parameter in the set.
		\param	isObligatory	indicate, that this parameter is obligatory.
	*/
	void Init(const IParamsSet* parameterSetPtr, const QByteArray& parameterId, bool isObligatory = true);

	/**
		Initialize the pointer with the component parameters.
		\param	parameterSetPtr	parameter set, parameter will be taken from this set.
		\param	parameterIdAttribute	component attribute storing ID of parameter in the set.
		\param	defaultRef				component reference used if parameter is not found in the set.
		\param	isObligatory			indicate, that this parameter is obligatory.
	*/
	void Init(	const IParamsSet* parameterSetPtr,
				const icomp::TAttributeMember<icomp::CIdAttribute>& parameterIdAttribute,
				const icomp::TReferenceMember<ParameterInterace>& defaultRef,
				bool isObligatory = true);
};


// public methods

template <class ParameterInterace>
TParamsPtr<ParameterInterace>::TParamsPtr(const ParameterInterace* ptr)
:	BaseClass(ptr)
{
}


template <class ParameterInterace>
TParamsPtr<ParameterInterace>::TParamsPtr(const IParamsSet* parameterSetPtr, const QByteArray& parameterId, bool isObligatory)
{
	Init(parameterSetPtr, parameterId, isObligatory);
}


template <class ParameterInterace>
TParamsPtr<ParameterInterace>::TParamsPtr(
			const IParamsSet* parameterSetPtr,
			const icomp::TAttributeMember<icomp::CIdAttribute>& parameterIdAttribute,
			const icomp::TReferenceMember<ParameterInterace>& defaultRef,
			bool isObligatory)
{
	Init(parameterSetPtr, parameterIdAttribute, defaultRef, isObligatory);
}


template <class ParameterInterace>
void TParamsPtr<ParameterInterace>::Init(const IParamsSet* parameterSetPtr, const QByteArray& parameterId, bool I_IF_DEBUG(isObligatory))
{
	if ((parameterSetPtr != NULL) && !parameterId.isEmpty()){
		BaseClass::SetPtr(dynamic_cast<const ParameterInterace*>(parameterSetPtr->GetParameter(parameterId)));

		I_IF_DEBUG(
			if (!BaseClass::IsValid() && isObligatory){
				iprm::IParamsSet::Ids existingParamIds = parameterSetPtr->GetParamIds();
				QStringList existingIds;
				for (iprm::IParamsSet::Ids::ConstIterator index = existingParamIds.constBegin(); index != existingParamIds.constEnd(); index++){
					existingIds.push_back(*index);
				}

				QString idList = existingIds.join(", ");
				QString debugMessage = QString("Parameter %1 was not found in the parameter set. Following parameter IDs are registered: %2").arg(QString(parameterId)).arg(idList);

				qDebug(debugMessage.toLocal8Bit().constData());
			}
		)
	}
	else{
		BaseClass::Reset();
	}
}


template <class ParameterInterace>
void TParamsPtr<ParameterInterace>::Init(
			const IParamsSet* parameterSetPtr,
			const icomp::TAttributeMember<icomp::CIdAttribute>& parameterIdAttribute,
			const icomp::TReferenceMember<ParameterInterace>& defaultRef,
			bool isObligatory)
{
	BaseClass::Reset();

	if (parameterSetPtr != NULL){
		if (parameterIdAttribute.IsValid()){
			BaseClass::SetPtr(dynamic_cast<const ParameterInterace*>(parameterSetPtr->GetParameter(*parameterIdAttribute)));
		}

		if (!BaseClass::IsValid() && defaultRef.IsValid()){
			BaseClass::SetPtr(defaultRef.GetPtr());
		}

		I_IF_DEBUG(
			if (!BaseClass::IsValid() && isObligatory){
				iprm::IParamsSet::Ids existingParamIds = parameterSetPtr->GetParamIds();
				QStringList existingIds;
				for (iprm::IParamsSet::Ids::ConstIterator index = existingParamIds.constBegin(); index != existingParamIds.constEnd(); index++){
					existingIds.push_back(*index);
				}

				QString idList = existingIds.join(", ");
				QString debugMessage = QString("Parameter %1 was not found in the parameter set and no default parameter is active. Following parameter IDs are registered: %2").arg(QString(parameterId)).arg(idList);

				qDebug(debugMessage.toLocal8Bit().constData());
			}
		)
	}
}


} // namespace iprm


#endif // !iprm_TParamsPtr_included


