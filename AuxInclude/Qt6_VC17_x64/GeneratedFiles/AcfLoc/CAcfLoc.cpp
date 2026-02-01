#include "CAcfLoc.h"


// ACF includes
#include <iser/CMemoryReadArchive.h>
#include <icomp/CRegistryElement.h>


CAcfLoc::CAcfLoc(const icomp::IRegistryElement* userMainElementPtr, bool manualAutoInit)
:	BaseClass(manualAutoInit)
{
	static icomp::CRegistryElement mainElement;
	static CMainRegistry mainRegistry;
	static CLocalEnvironmentManager localEnvironmentManager;

	static icomp::CCompositeComponentStaticInfo mainComponentStaticInfo(mainRegistry, localEnvironmentManager, NULL);

	m_mainContextPtr.reset(new icomp::CCompositeComponentContext((userMainElementPtr != NULL)? userMainElementPtr: &mainElement, &mainComponentStaticInfo, &mainRegistry, &localEnvironmentManager, NULL, ""));

	SetComponentContext(m_mainContextPtr, NULL, false);
}


// Embedded class CMainRegistry

CAcfLoc::CMainRegistry::CMainRegistry()
{
	// translations
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Log XML file"); // Attribute TypeDescriptions in XmlLogSerializer
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Log"); // Attribute DockTitle in LogDock
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Lazy(-1)"); // Attribute Names in Lazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Lazy(-1)_AP"); // Attribute Names in Lazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Lazy(0)"); // Attribute Names in Lazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Lazy(0)_AP"); // Attribute Names in Lazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Lazy"); // Attribute Names in ComposedGui
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "NoLazy"); // Attribute Names in ComposedGui
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "NoLazy(-1)"); // Attribute Names in NoLazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "NoLazy(-1)_AP"); // Attribute Names in NoLazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "NoLazy(0)"); // Attribute Names in NoLazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "NoLazy(0)_AP"); // Attribute Names in NoLazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Lazy(-1)"); // Attribute Names in Lazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Lazy(-1)_AP"); // Attribute Names in Lazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Lazy(0)"); // Attribute Names in Lazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Lazy(0)_AP"); // Attribute Names in Lazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Lazy"); // Attribute Names in ComposedGui
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "NoLazy"); // Attribute Names in ComposedGui
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "NoLazy(-1)"); // Attribute Names in NoLazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "NoLazy(-1)_AP"); // Attribute Names in NoLazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "NoLazy(0)"); // Attribute Names in NoLazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "NoLazy(0)_AP"); // Attribute Names in NoLazy
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Page1"); // Attribute Names in ComposedGui_2
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Page2"); // Attribute Names in ComposedGui_2
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Page1"); // Attribute Names in ComposedGui_1
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Page2"); // Attribute Names in ComposedGui_1
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Page1"); // Attribute Names in ComposedGui
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Page2"); // Attribute Names in ComposedGui
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "ACF"); // Attribute VersionIdDesc in BaseInfo
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "About"); // Attribute DialogTitle in DialogGui
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "Files"); // Attribute DockTitle in ExplorerDock
	#undef TRANSLATED_TEXT
	#define TRANSLATED_TEXT QT_TRANSLATE_NOOP("Attribute", "File Selector"); // Attribute DockTitle in DockWidgetGui
	#undef TRANSLATED_TEXT

	static const unsigned char data[] = {
				0x70, 0xa1, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8f, 0x5e, 0xfc, 0xff, 0x1d, 0xa7, 0x0, 0x0, 
				0x0, 0x0, 0x0, 0x0, 0xe2, 0x58, 0xff, 0xff, 0xd0, 0x2a, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 
				0x2f, 0xd5, 0xf8, 0xff, 0xdc, 0x49, 0xe0, 0x1b, 0x0, 0x0, 0x0, 0x0, 0x23, 0xb6, 0x1f, 0xe4, 
				0x45, 0x5f, 0xaf, 0x1, 0x0, 0x0, 0x0, 0x0, 0xba, 0xa0, 0x50, 0xfe, 0x43, 0xe7, 0xc5, 0x1, 
				0x0, 0x0, 0x0, 0x0, 0xbc, 0x18, 0x3a, 0xfe};

	iser::CMemoryReadArchive archive(data, sizeof(data), false);

	Serialize(archive);
}


// Embedded class CLocalEnvironmentManager

CAcfLoc::CLocalEnvironmentManager::CLocalEnvironmentManager()
{
	static const unsigned char data[] = {
				0xac, 0xa0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x53, 0x5f, 0xff, 0xff};

	iser::CMemoryReadArchive archive(data, sizeof(data), false);

	Serialize(archive);
}


