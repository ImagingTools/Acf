#include "iqt/CTranslationManagerComp.h"


// Qt includes
#include <QApplication>
#include <QIcon>
#include <QLocale>
#include <QDir>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/iqt.h"


namespace iqt
{


// public methods

CTranslationManagerComp::CTranslationManagerComp()
	:m_currentLanguageIndex(-1)
{
}


// reimplemented (icomp::CComponentBase)

void CTranslationManagerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	// create and install translations:
	if (m_languagesAttrPtr.IsValid() && m_translationFilePrefixAttrPtr.IsValid()){
		int languagesCount = GetLanguagesCount();

		QString translationsPath = iqt::GetQString(*m_translationFilePathAttrPtr);
		QString translationFilePrefix = iqt::GetQString(*m_translationFilePrefixAttrPtr);

		for(int translatorIndex = 0; translatorIndex < languagesCount; translatorIndex++){
			QString languageId = iqt::GetQString(m_languagesAttrPtr[translatorIndex]);

			QString translatorFile = translationsPath + QString("/") + translationFilePrefix + QString("_") + languageId;
			
			istd::TDelPtr<QTranslator> qtTranslatorPtr(new QTranslator(qApp));
			
			qtTranslatorPtr->load(translatorFile);

			m_translatorsList.PushBack(qtTranslatorPtr.PopPtr());
		}
	}
}


// reimplemented (ITranslationManager)

int CTranslationManagerComp::GetLanguagesCount() const
{
	if (m_languagesAttrPtr.IsValid()){
		return m_languagesAttrPtr.GetCount();
	}

	return 0;
}


const QTranslator* CTranslationManagerComp::GetLanguageTranslator(int languageIndex) const
{
	I_ASSERT(languageIndex >= 0);
	I_ASSERT(languageIndex < languageIndex);

	return m_translatorsList.GetAt(languageIndex);
}


const ITranslationManager* CTranslationManagerComp::GetSlaveTranslationManager() const
{
	return m_slaveTranslationManagerCompPtr.GetPtr();
}


void CTranslationManagerComp::SwitchLanguage(int languageIndex)
{
	if (m_currentLanguageIndex == languageIndex){
		return;
	}

	istd::CChangeNotifier changePtr(this, iprm::ISelectionParam::CF_SELECTION_CHANGED);

	if (m_currentLanguageIndex >= 0){
		QCoreApplication::removeTranslator(m_translatorsList.GetAt(languageIndex));
	}

	if (languageIndex >= 0){
		QCoreApplication::installTranslator(m_translatorsList.GetAt(languageIndex));
	}

	m_currentLanguageIndex = languageIndex;

	if (m_slaveTranslationManagerCompPtr.IsValid()){
		m_slaveTranslationManagerCompPtr->SwitchLanguage(languageIndex);
	}
}


void CTranslationManagerComp::SetSystemLanguage()
{
	QLocale local = QLocale::system();
	QString defaultSystemLanguage = local.name();

	int languagesCount = GetLanguagesCount();

	for(int translatorIndex = 0; translatorIndex < languagesCount; translatorIndex++){
		QString languageId = iqt::GetQString(m_languagesAttrPtr[translatorIndex]);
		if (languageId == defaultSystemLanguage){
			SwitchLanguage(translatorIndex);
			break;
		}
	}
}


// reimplemented (iprm::ISelectionParam)

const iprm::ISelectionConstraints* CTranslationManagerComp::GetConstraints() const
{
	return this;
}


int CTranslationManagerComp::GetSelectedOptionIndex() const
{
	return m_currentLanguageIndex;
}


bool CTranslationManagerComp::SetSelectedOptionIndex(int index)
{
	SwitchLanguage(index);

	return true;
}


iprm::ISelectionParam* CTranslationManagerComp::GetActiveSubselection() const
{
	return NULL;
}


// reimplemented (iprm::ISelectionParam)

int CTranslationManagerComp::GetOptionsCount() const
{
	return GetLanguagesCount();
}


istd::CString CTranslationManagerComp::GetOptionName(int index) const
{
	I_ASSERT(index < GetOptionsCount());

	if (index >= 0 && m_languagesAttrPtr.IsValid()){
		QString languageId = iqt::GetQString(m_languagesAttrPtr[index]);

		QLocale locale(languageId);

		return iqt::GetCString(QLocale::languageToString(locale.language()));
	}

	return istd::CString();
}


// reimplemented (iser::ISerializable)

bool CTranslationManagerComp::Serialize(iser::IArchive& archive)
{
	int currentIndex = m_currentLanguageIndex;
	static iser::CArchiveTag selectedLanguageIndexTag("LanguageIndex", "Selected language index");
	bool retVal = archive.BeginTag(selectedLanguageIndexTag);
	retVal = retVal && archive.Process(currentIndex);
	retVal = retVal && archive.EndTag(selectedLanguageIndexTag);

	if (!archive.IsStoring()){
		SwitchLanguage(currentIndex);
	}

	return retVal;
}


} // namespace iqt


