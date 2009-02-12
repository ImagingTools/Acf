#include "iqt/CDefaultRegistryLoaderProvider.h"



namespace iqt
{


CDefaultRegistryLoaderProvider::CDefaultRegistryLoaderProvider()
{
	m_registryLoaderComp.InitComponent();

	m_packagesLoaderComp.SetRef("RegistryLoader", &m_registryLoaderComp);
	m_packagesLoaderComp.InitComponent();
}


} // namespace iqt


