#ifndef icmpstr_CRegistryPreviewComp_included
#define icmpstr_CRegistryPreviewComp_included


#include <QProcess>
#include <QLabel>


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

	I_BEGIN_COMPONENT(CRegistryPreviewComp)
		I_REGISTER_INTERFACE(icmpstr::IRegistryPreview)
	I_END_COMPONENT

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
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryPreviewComp_included


