#ifndef iqt_CTranslationManagerComp_included
#define iqt_CTranslationManagerComp_included


// Qt includes
#include <QMap>
#include <QString>
#include <QTranslator>


// ACF includes
#include "istd/TPointerVector.h"

#include "icomp/CComponentBase.h"

#include "iprm/ISelectionParam.h"
#include "iprm/ISelectionConstraints.h"

#include "iqt/ITranslationManager.h"


namespace iqt
{


class CTranslationManagerComp:
			public icomp::CComponentBase, 
			virtual public iqt::ITranslationManager,
			virtual public iprm::ISelectionParam,
			virtual public iprm::ISelectionConstraints
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTranslationManagerComp);
		I_REGISTER_INTERFACE(iqt::ITranslationManager);
		I_REGISTER_INTERFACE(iprm::ISelectionParam);
		I_REGISTER_INTERFACE(iprm::ISelectionConstraints);
		I_ASSIGN(m_translationFilePathAttrPtr, "TranslationFilePath", "Base file path got translations", true, "");
		I_ASSIGN(m_translationFilePrefixAttrPtr, "TranslationFilePrefix", "Prefix of the translation file", true, "");
		I_ASSIGN_MULTI_0(m_languagesAttrPtr, "LanguageIds", "List of language IDs in ISO-format", true);
		I_ASSIGN(m_slaveTranslationManagerCompPtr, "SlaveTranslationManager", "Slave translation manager", false, "SlaveTranslationManager");
	I_END_COMPONENT;

	CTranslationManagerComp();

	// reimplemented (icomp:CComponentBase)
	virtual void OnComponentCreated();

	// reimplemented (iqt::ITranslationManager)
	virtual int GetLanguagesCount() const;
	virtual const QTranslator* GetLanguageTranslator(int languageIndex) const;
	virtual const ITranslationManager* GetSlaveTranslationManager() const;
	virtual void SwitchLanguage(int languageIndex);
	virtual void SetSystemLanguage();

	// reimplemented (iprm::ISelectionParam)
	virtual const iprm::ISelectionConstraints* GetConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual iprm::ISelectionParam* GetActiveSubselection() const;

	// reimplemented (iprm::ISelectionParam)
	virtual int GetOptionsCount() const;
	virtual istd::CString GetOptionName(int index) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	I_ATTR(istd::CString, m_translationFilePathAttrPtr);
	I_ATTR(istd::CString, m_translationFilePrefixAttrPtr);
	I_MULTIATTR(istd::CString, m_languagesAttrPtr);
	I_REF(iqt::ITranslationManager, m_slaveTranslationManagerCompPtr);

	typedef istd::TPointerVector<QTranslator> TranslatorsList;

	TranslatorsList m_translatorsList;

	int m_currentLanguageIndex;
};


} // namespace iqt


#endif // !iqt_CTranslationManagerComp_included


