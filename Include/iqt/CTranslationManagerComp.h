// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QTranslator>
#include <QtCore/QReadWriteLock>

// ACF includes
#include <istd/TSmartPtr.h>
#include <icomp/CComponentBase.h>
#include <imod/TSingleModelObserverBase.h>
#include <iprm/ISelectionParam.h>
#include <iprm/IOptionsList.h>
#include <iqt/ITranslationManager.h>
#include <ilog/TLoggerCompWrap.h>


namespace iqt
{


/**
	A translation manager component.

	Translation manager based on using a set of Qt translation files.
*/
class CTranslationManagerComp:
			public QObject,
			public ilog::CLoggerComponentBase,
			virtual public iqt::ITranslationManager,
			virtual public iprm::IOptionsList
{
	Q_OBJECT
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTranslationManagerComp);
		I_REGISTER_INTERFACE(iqt::ITranslationManager);
		I_REGISTER_INTERFACE(iprm::IOptionsList);
		I_ASSIGN(m_ignoreLocaleAttrPtr, "IgnoreLocale", "Ignore locale in language IDs", true, false);
		I_ASSIGN(m_installTranslatorAttrPtr, "InstallTranslator", "Install translator when switch language", false, false);
		I_ASSIGN(m_translationFilePathAttrPtr, "TranslationFilePath", "Base file path got translations", true, "");
		I_ASSIGN(m_translationFilePrefixAttrPtr, "TranslationFilePrefix", "Prefix of the translation file", true, "");
		I_ASSIGN_MULTI_0(m_languagesAttrPtr, "LanguageIds", "List of language IDs in ISO-format", true);
		I_ASSIGN_MULTI_0(m_languageNamesAttrPtr, "LanguageNames", "List of language names according to 'LanguageIds' attribute", false);
		I_ASSIGN(m_slaveTranslationManagerCompPtr, "SlaveTranslationManager", "Slave translation manager", false, "SlaveTranslationManager");
		I_ASSIGN(m_languageSelectionCompPtr, "LanguageSelection", "Language selection provider", false, "LanguageSelection");
		I_ASSIGN_TO(m_languageSelectionModelCompPtr, m_languageSelectionCompPtr, false);
	I_END_COMPONENT;

	/**
		Default constructor.
	*/
	CTranslationManagerComp();

	// reimplemented (icomp:CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

	// reimplemented (iqt::ITranslationManager)
	virtual const iprm::IOptionsList& GetLanguagesInfo() const override;
	virtual int GetCurrentLanguageIndex() const override;
	virtual const QTranslator* GetLanguageTranslator(int languageIndex) const override;
	virtual const ITranslationManager* GetSlaveTranslationManager() const override;
	virtual void SwitchLanguage(int languageIndex) override;
	virtual void SetSystemLanguage() override;

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const override;
	virtual int GetOptionsCount() const override;
	virtual QString GetOptionName(int index) const override;
	virtual QString GetOptionDescription(int index) const override;
	virtual QByteArray GetOptionId(int index) const override;
	virtual bool IsOptionEnabled(int index) const override;

protected:
	virtual void LoadTranslations();

Q_SIGNALS:
	void EmitInstallTranslator(int languageIndex);

protected	Q_SLOTS:
	void InstallTranslator(int languageIndex);

protected:
	/**
		\internal
		Internally used language selection observer.
	*/
	class LanguageSelectionObserver: public imod::TSingleModelObserverBase<iprm::ISelectionParam>
	{
	public:
		explicit LanguageSelectionObserver(CTranslationManagerComp& parent);

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override;

	private:
		CTranslationManagerComp& m_parent;
	};

protected:
	I_ATTR(bool, m_ignoreLocaleAttrPtr);
	I_ATTR(bool, m_installTranslatorAttrPtr);
	I_ATTR(QString, m_translationFilePathAttrPtr);
	I_ATTR(QByteArray, m_translationFilePrefixAttrPtr);
	I_MULTIATTR(QByteArray, m_languagesAttrPtr);
	I_MULTITEXTATTR(m_languageNamesAttrPtr);
	I_REF(iqt::ITranslationManager, m_slaveTranslationManagerCompPtr);
	I_REF(iprm::ISelectionParam, m_languageSelectionCompPtr);
	I_REF(imod::IModel, m_languageSelectionModelCompPtr);

	struct Translator
	{
		istd::TSmartPtr<QTranslator> translatorPtr;
		QString languageName;
		QByteArray languageId;
	};

	typedef QVector<Translator> TranslatorsList;

	TranslatorsList m_translatorsList;

	LanguageSelectionObserver m_selectionObserver;

	int m_currentLanguageIndex;

	mutable QReadWriteLock m_mutex;
};


} // namespace iqt




