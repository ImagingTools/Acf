#ifndef icomp_TAttributeMember_included
#define icomp_TAttributeMember_included


// Qt includes
#include <QtCore/QCoreApplication>

// ACF includes
#include "icomp/IComponentContext.h"
#include "icomp/IRealAttributeStaticInfo.h"
#include "icomp/TAttribute.h"
#include "icomp/CComponentContext.h"


namespace icomp
{


/**
	Pointer to component attribute.
	\internal
	Don't use direct this class, use macros I_ATTR and I_ASSIGN instead.
*/
template <typename Attribute>
class TAttributeMemberBase
{
public:
	typedef Attribute AttributeType;
	typedef typename Attribute::ValueType AttributeValueType;
	typedef void InterfaceType;

	TAttributeMemberBase();

	/**
		Initialize this attribute.
		\param	ownerPtr				pointer to parent component of this attribute.
		\param	staticInfo				static info structure creating this attribute.
		\param	definitionComponentPtr	optional pointer will be set with pointer to component defining this attribute.
										If this attribute was exported it will differ from parent component.
	*/
	bool Init(	const IComponent* ownerPtr,
				const IRealAttributeStaticInfo& staticInfo,
				const IComponent** definitionComponentPtr = NULL);

	/**
		Check if this attribute is valid.
	*/
	bool IsValid() const;

	/**
		Access to internal attribute pointer.
	*/
	const Attribute* GetAttributePtr() const;

	/**
		Access to internal attribute pointer.
	*/
	const Attribute* operator->() const;

	/**
		Access to object pointed by internal pointer.
	*/
	const AttributeValueType& operator*() const;

protected:
	void SetAttribute(const Attribute* attributePtr);

private:
	static QByteArray GetContextId(const IComponentContext& context);

private:
	const Attribute* m_attributePtr;
};


// public methods

template <typename Attribute>
TAttributeMemberBase<Attribute>::TAttributeMemberBase()
:	m_attributePtr(NULL)
{
}


template <typename Attribute>
bool TAttributeMemberBase<Attribute>::Init(
			const IComponent* ownerPtr,
			const IRealAttributeStaticInfo& staticInfo,
			const IComponent** definitionComponentPtr)
{
	I_ASSERT(ownerPtr != NULL);

	const QByteArray& attributeId = staticInfo.GetAttributeId();
	const IComponentContext* componentContextPtr = ownerPtr->GetComponentContext();
	if (componentContextPtr != NULL){
		if (definitionComponentPtr != NULL){
			int definitionLevel = -1;
			const iser::IObject* attributePtr = componentContextPtr->GetAttribute(attributeId, &definitionLevel);
			if (attributePtr != NULL){
				m_attributePtr = dynamic_cast<const Attribute*>(attributePtr);

				if (m_attributePtr != NULL){
					I_ASSERT(definitionLevel >= 0);

					while (definitionLevel > 0){
						ownerPtr = ownerPtr->GetParentComponent();
						I_ASSERT(ownerPtr != NULL);

						--definitionLevel;
					}

					*definitionComponentPtr = ownerPtr;

					return true;
				}
				else{
					qCritical(	"Component %s: Attribute %s type inconsistence!",
								GetContextId(*componentContextPtr).constData(),
								attributeId.constData());
				}
			}
		}
		else{
			const iser::IObject* attributePtr = componentContextPtr->GetAttribute(attributeId, NULL);
			m_attributePtr = dynamic_cast<const Attribute*>(attributePtr);

			if (m_attributePtr == NULL){
				if (attributePtr != NULL){
					qCritical("Component %s: Attribute %s exists in the component context but has a wrong type",
								GetContextId(*componentContextPtr).constData(),
								attributeId.constData());
				}
			}

			return (m_attributePtr != NULL);
		}
	}
	else{
		qCritical("Error during resolving of attribute: %s in component %s: Component context not set", GetContextId(*componentContextPtr).constData(), attributeId.constData());
		
		m_attributePtr = NULL;
	}

	return false;
}


template <typename Attribute>
bool TAttributeMemberBase<Attribute>::IsValid() const
{
	return (m_attributePtr != NULL);
}


template <typename Attribute>
const Attribute* TAttributeMemberBase<Attribute>::GetAttributePtr() const
{
	return m_attributePtr;
}


template <typename Attribute>
const Attribute* TAttributeMemberBase<Attribute>::operator->() const
{
	return m_attributePtr;
}


template <typename Attribute>
const typename TAttributeMemberBase<Attribute>::AttributeValueType& TAttributeMemberBase<Attribute>::operator*() const
{
	I_ASSERT(m_attributePtr != NULL);	// operator* was called for invalid object, or no IsValid() check was called.

	return m_attributePtr->GetValue();
}


// protected methods

template <typename Attribute>
void TAttributeMemberBase<Attribute>::SetAttribute(const Attribute* attributePtr)
{
	m_attributePtr = attributePtr;
}


// private static methods

template <typename Attribute>
QByteArray TAttributeMemberBase<Attribute>::GetContextId(const IComponentContext& context)
{
	QByteArray completeIdPath = context.GetContextId();
	const IComponentContext* parentContextPtr = context.GetParentContext();

	while ((parentContextPtr != NULL) && (parentContextPtr != &context)){
		completeIdPath = parentContextPtr->GetContextId() + "/" + completeIdPath;
	
		parentContextPtr = parentContextPtr->GetParentContext();
	}

	return completeIdPath; 
}


// other constructs used for special template for QString attribute

template <typename Attribute>
class TAttributeMember: public TAttributeMemberBase<Attribute>
{
};


template <>
class TAttributeMember< TAttribute<QString> >: public TAttributeMemberBase< TAttribute<QString> >
{
public:
	QString operator*() const
	{
		return QCoreApplication::translate("Attribute", TAttributeMemberBase< TAttribute<QString> >::operator*().toAscii());
	}
};


} // namespace icomp


#endif // !icomp_TAttributeMember_included


