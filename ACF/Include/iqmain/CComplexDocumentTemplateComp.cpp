#include "iqmain/CComplexDocumentTemplateComp.h"


namespace iqmain
{


CComplexDocumentTemplateComp::CComplexDocumentTemplateComp()
:	m_globalMenuCommands("Global", NULL)
{
	m_globalMenuCommands.SetEnabled(false);
}


// reimplemented (idoc::IDocumentTemplate)

const idoc::IHierarchicalCommand* CComplexDocumentTemplateComp::GetGlobalMenuCommands() const
{
	return &m_globalMenuCommands;
}


} // namespace iqmain


