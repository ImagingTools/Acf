#include "iqt/CCompactXmlFileReadArchive.h"


// include STL
#include <cstring>

// Qt includes
#include <QtCore/QFile>
#include <QtXml/QDomNodeList>


namespace iqt
{


CCompactXmlFileReadArchive::CCompactXmlFileReadArchive(
			const QString& filePath,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	m_serializeHeader(serializeHeader),
	m_rootTag(rootTag)
{
	if (!filePath.isEmpty()){
		OpenFile(filePath);
	}
}


bool CCompactXmlFileReadArchive::OpenFile(const QString& filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		return false;
	}

	if (!m_document.setContent(&file)){
		file.close();

		return false;
	}

	if (m_currentParent.nodeValue() != m_rootTag.GetId()){
		QDomElement mainElement = m_document.documentElement();

		m_currentParent = mainElement;
	}

	bool retVal = !m_currentParent.isNull();

	if (m_serializeHeader){
		retVal = retVal && SerializeAcfHeader();
	}

	return retVal;
}


// reimplemented (iser::IArchive)

bool CCompactXmlFileReadArchive::IsTagSkippingSupported() const
{
	return true;
}


bool CCompactXmlFileReadArchive::BeginTag(const iser::CArchiveTag& tag)
{
	Q_ASSERT(m_currentAttribute.isEmpty());

	QString tagId(tag.GetId());

	m_currentAttribute.clear();

	if (m_tagsStack.isEmpty() || (m_tagsStack.back() == NULL) || (m_tagsStack.back()->GetTagType() != iser::CArchiveTag::TT_MULTIPLE)){
		int tagType = tag.GetTagType();
		if (tagType == iser::CArchiveTag::TT_LEAF){
			m_currentAttribute = tag.GetId();

			m_tagsStack.push_back(NULL);

			return true;
		}
		else if (tagType == iser::CArchiveTag::TT_WEAK){
			m_tagsStack.push_back(NULL);

			return true;
		}
	}

	QDomElement element = m_currentParent.firstChildElement(tagId);
	if (!element.isNull()){
		m_currentParent = element;
	}

	m_tagsStack.push_back(&tag);

	return !element.isNull();
}


bool CCompactXmlFileReadArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count)
{
	QString tagId(tag.GetId());

	QDomElement element = m_currentParent.firstChildElement(tagId);
	if (!element.isNull()){
		m_currentParent = element;
	}
	else{
		return false;
	}

	int tempCount = 0;
	QDomElement child = element.firstChildElement(QString(subTag.GetId()));
	while (!child.isNull()){
		tempCount++;
		child = child.nextSiblingElement(QString(subTag.GetId()));
	}
	count = tempCount;

	m_tagsStack.push_back(&tag);

	return !element.isNull();
}


bool CCompactXmlFileReadArchive::EndTag(const iser::CArchiveTag& /*tag*/)
{
	m_currentAttribute.clear();

	if (m_tagsStack.isEmpty()){
		return false;
	}

	const iser::CArchiveTag* lastTagPtr = m_tagsStack.back();
	m_tagsStack.pop_back();

	if (lastTagPtr == NULL){
		return true;
	}

	QDomNode parent = m_currentParent.parentNode();
	
	parent.removeChild(m_currentParent);

	m_currentParent = parent.toElement();

	return !m_currentParent.isNull();
}


bool CCompactXmlFileReadArchive::Process(QString& value)
{
	return ReadStringNode(value);
}


// protected methods

bool CCompactXmlFileReadArchive::ReadStringNode(QString& text)
{
	if (m_currentAttribute.isEmpty()){
		QDomNode node = m_currentParent.firstChild();
		//Kill separator tags (<br/>)
		while (node.nodeName() == "br"){
			QDomNode brNode = node;
			node = node.nextSibling();
			m_currentParent.removeChild(brNode);
		}
	
		text = node.nodeValue();

		m_currentParent.removeChild(node);
	}
	else{
		text = m_currentParent.attribute(m_currentAttribute);
	}

	return !m_currentParent.isNull();
}


// reimplemented (iser::CTextReadArchiveBase)

bool CCompactXmlFileReadArchive::ReadTextNode(QByteArray& text)
{
	QString stringText;

	if (ReadStringNode(stringText)){
		text = stringText.toLocal8Bit();

		return true;
	}

	return false;
}


} // namespace iqt


