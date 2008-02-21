#ifndef icomp_included
#define icomp_included



/**
\defgroup icomp Component Module
\ingroup ACF
@{
*/

/** 
@} 
*/


/**
	General ACF component declaration.
	It should be placed in class declaration as first line.
*/
#define I_COMPONENT(BaseClass)\
	public:\
		static inline icomp::IComponentStaticInfo& GetStaticInfo()\
		{\
			static icomp::CComponentStaticInfo staticInfo(BaseClass::GetStaticInfo());\

			return staticInfo;\
		}\
		// reimplemented (icomp::IComponent)
		virtual icomp::IComponentStaticInfo* GetStaticInfoPtr()\
		{\
			return &GetStaticInfo();\
		}\
	private:

#define I_REGISTER_INTERFACE(InterfaceType)\
	static inline void Register##InterfaceType()\
	{\
		static icomp::TInterfaceRegistrator<InterfaceType> staticInfo(GetStaticInfo());\
	}\

#define I_ATTR(attrType, member)\
	TAttributePtr<attrType> member;

#define I_REF(interfaceType, member)\
	TReferencePtr<interfaceType> member;

#define I_MULTIREF(interfaceType, member)\
	TMultiReferencePtr<interfaceType> member;

#define I_FACT(interfaceType, member)\
	TFactoryPtr<interfaceType> member;

#define I_MULTIFACT(interfaceType, member)\
	TMultiFactoryPtr<interfaceType> member;

#define I_ASSIGN(member, context, id, isObligatory)\
	{\
		static TAttributeStaticInfo<member::InterfaceType> info(GetStaticInfo(), id, isObligatory);\
		member.Init(this, context, info);\
	}


/**
	Package with interfaces and class used for components concept.
	This package is system independent.
	This package use following ACF packages:
	\li istd
	\li iser
*/
namespace icomp
{


} // namespace icomp


#endif // !icomp_included


