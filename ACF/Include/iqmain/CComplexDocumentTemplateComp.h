#ifndef iqmain_CComplexDocumentTemplateComp_included
#define iqmain_CComplexDocumentTemplateComp_included


#include "idoc/CSerializedDocumentTemplateComp.h"

#include "iqt/IGuiObject.h"
#include "iqt/CHierarchicalCommand.h"

#include "iqmain/iqmain.h"


namespace iqmain
{


/**
	Extension of \c idoc::CSerializedDocumentTemplateComp providing additional Qt-specified funktionality.
*/
class CComplexDocumentTemplateComp: public idoc::CSerializedDocumentTemplateComp
{
public:
	typedef idoc::CSerializedDocumentTemplateComp BaseClass;

	CComplexDocumentTemplateComp();

	I_BEGIN_COMPONENT(CComplexDocumentTemplateComp)
		I_ASSIGN(m_aboutCommandTextAttrPtr, "AboutCommandText", "Text used in about menu", false, "Document");
		I_ASSIGN(m_aboutGuiCompPtr, "AboutGui", "GUI used inside about dialog box", false, "AboutGui");
	I_END_COMPONENT

	// reimplemented (idoc::IDocumentTemplate)
	virtual const idoc::IHierarchicalCommand* GetGlobalMenuCommands() const;

private:
	I_ATTR(istd::CString, m_aboutCommandTextAttrPtr);
	I_REF(iqt::IGuiObject, m_aboutGuiCompPtr);

	iqt::CHierarchicalCommand m_globalMenuCommands;
};


} // namespace iqmain


#endif // !iqmain_CComplexDocumentTemplateComp_included


