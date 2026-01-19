// ACF includes
#include "CPrinterProfile.h"
#include <icmm/ISpectralColorSpecification.h>

// Qt includes
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>


namespace iimgprint
{


class CPrinterProfile::Impl
{
public:
	PrinterColorSpace colorSpace;
	QString printerName;
	icmm::ISpectralColorSpecification::ConstColorSpecPtr spectralData;
	double gamutCoverage;
	
	Impl()
		: colorSpace(PrinterColorSpace::RGB)
		, gamutCoverage(1.0)
	{
	}
};


CPrinterProfile::CPrinterProfile()
	: m_impl(new Impl())
{
}


CPrinterProfile::CPrinterProfile(
	icmm::ISpectralColorSpecification::ConstColorSpecPtr spectralData,
	PrinterColorSpace colorSpace,
	const QString& printerName)
	: m_impl(new Impl())
{
	m_impl->spectralData = spectralData;
	m_impl->colorSpace = colorSpace;
	m_impl->printerName = printerName;
	
	// Calculate gamut coverage based on spectral data
	// This is a simplified estimation
	if (spectralData) {
		m_impl->gamutCoverage = 0.85; // Placeholder - would calculate from spectral range
	}
}


CPrinterProfile::CPrinterProfile(const CPrinterProfile& other)
	: m_impl(new Impl(*other.m_impl))
{
}


CPrinterProfile& CPrinterProfile::operator=(const CPrinterProfile& other)
{
	if (this != &other) {
		*m_impl = *other.m_impl;
	}
	return *this;
}


CPrinterProfile::~CPrinterProfile()
{
	delete m_impl;
}


PrinterColorSpace CPrinterProfile::GetColorSpace() const
{
	return m_impl->colorSpace;
}


void CPrinterProfile::SetColorSpace(PrinterColorSpace colorSpace)
{
	m_impl->colorSpace = colorSpace;
}


QString CPrinterProfile::GetPrinterName() const
{
	return m_impl->printerName;
}


void CPrinterProfile::SetPrinterName(const QString& name)
{
	m_impl->printerName = name;
}


bool CPrinterProfile::HasSpectralData() const
{
	return m_impl->spectralData != nullptr;
}


icmm::ISpectralColorSpecification::ConstColorSpecPtr CPrinterProfile::GetSpectralSpecification() const
{
	return m_impl->spectralData;
}


void CPrinterProfile::SetSpectralSpecification(icmm::ISpectralColorSpecification::ConstColorSpecPtr spectralData)
{
	m_impl->spectralData = spectralData;
	
	// Recalculate gamut coverage
	if (spectralData) {
		m_impl->gamutCoverage = 0.85; // Placeholder
	}
}


double CPrinterProfile::GetGamutCoverage() const
{
	return m_impl->gamutCoverage;
}


bool CPrinterProfile::IsValid() const
{
	return m_impl->spectralData != nullptr;
}


bool CPrinterProfile::SaveToFile(const QString& filePath) const
{
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return false;
	}
	
	QXmlStreamWriter xml(&file);
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	
	xml.writeStartElement("PrinterProfile");
	xml.writeAttribute("version", "1.0");
	
	xml.writeTextElement("PrinterName", m_impl->printerName);
	xml.writeTextElement("ColorSpace", QString::number(static_cast<int>(m_impl->colorSpace)));
	xml.writeTextElement("GamutCoverage", QString::number(m_impl->gamutCoverage));
	
	// Note: Spectral data serialization would require more complex handling
	// This is a simplified version
	xml.writeStartElement("SpectralData");
	xml.writeAttribute("hasData", m_impl->spectralData ? "true" : "false");
	xml.writeEndElement(); // SpectralData
	
	xml.writeEndElement(); // PrinterProfile
	xml.writeEndDocument();
	
	file.close();
	return true;
}


bool CPrinterProfile::LoadFromFile(const QString& filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}
	
	QXmlStreamReader xml(&file);
	
	while (!xml.atEnd()) {
		xml.readNext();
		
		if (xml.isStartElement()) {
			if (xml.name() == QStringLiteral("PrinterName")) {
				m_impl->printerName = xml.readElementText();
			}
			else if (xml.name() == QStringLiteral("ColorSpace")) {
				int value = xml.readElementText().toInt();
				m_impl->colorSpace = static_cast<PrinterColorSpace>(value);
			}
			else if (xml.name() == QStringLiteral("GamutCoverage")) {
				m_impl->gamutCoverage = xml.readElementText().toDouble();
			}
		}
	}
	
	if (xml.hasError()) {
		file.close();
		return false;
	}
	
	file.close();
	return true;
}


} // namespace iimgprint
