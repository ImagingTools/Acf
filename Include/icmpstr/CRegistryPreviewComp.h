#ifndef icmpstr_CRegistryPreviewComp_included
#define icmpstr_CRegistryPreviewComp_included


// Qt includes
#include <QProcess>
#include <QLabel>

#include "icomp/IRegistryLoader.h"
#include "icomp/CComponentBase.h"

#include "icmpstr/IRegistryPreview.h"


namespace icmpstr
{


class CRegistryPreviewComp:
			public QObject,
			public icomp::CComponentBase,
			public IRegistryPreview
{
	Q_OBJECT
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CRegistryPreviewComp);
		I_REGISTER_INTERFACE(icmpstr::IRegistryPreview);
		I_ASSIGN(m_commandAttrPtr, "Command", "Command to start preview", true, "Acf");
		I_ASSIGN(m_registryLoaderCompPtr, "RegistryLoader", "Registry loader used to retrive package configuration", false, "RegistryLoader");
	I_END_COMPONENT;

	CRegistryPreviewComp();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (icmpstr::IRegistryPreview)
	virtual bool StartRegistry(const icomp::IRegistry& registry);
	virtual bool IsRunning() const;
	virtual void AbortRegistry();

protected slots:
	void OnStateChanged(QProcess::ProcessState state);

private:
	QString m_tempFileName;
	QProcess m_process;

	bool m_isRunning;

	I_ATTR(istd::CString, m_commandAttrPtr);
	I_REF(icomp::IRegistryLoader, m_registryLoaderCompPtr);
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryPreviewComp_included


