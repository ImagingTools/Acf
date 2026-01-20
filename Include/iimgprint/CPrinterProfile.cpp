// Qt includes
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

// ACF includes
#include "CPrinterProfile.h"
#include "CDeviceLabTransformation.h"
#include <icmm/ISpectralColorSpecification.h>


namespace iimgprint
{

// Constants for gamut coverage estimation
namespace {
	const double DEFAULT_GAMUT_COVERAGE = 0.85; // Placeholder for estimated gamut coverage
}


class CPrinterProfile::Impl
{
public:
	PrinterColorSpace m_colorSpace;
	QString m_printerName;
	icmm::ISpectralColorSpecification::ConstColorSpecPtr m_spectralData;
	double m_gamutCoverage;
	
	Impl()
		: m_colorSpace(PrinterColorSpace::RGB)
		, m_gamutCoverage(1.0)
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
	m_impl->m_spectralData = spectralData;
	m_impl->m_colorSpace = colorSpace;
	m_impl->m_printerName = printerName;
	
	// Calculate gamut coverage based on spectral data
	// This is a simplified estimation
	if (spectralData) {
		m_impl->m_gamutCoverage = DEFAULT_GAMUT_COVERAGE; // Placeholder - would calculate from spectral range
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
	return m_impl->m_colorSpace;
}


void CPrinterProfile::SetColorSpace(PrinterColorSpace colorSpace)
{
	m_impl->m_colorSpace = colorSpace;
}


QString CPrinterProfile::GetPrinterName() const
{
	return m_impl->m_printerName;
}


void CPrinterProfile::SetPrinterName(const QString& name)
{
	m_impl->m_printerName = name;
}


bool CPrinterProfile::HasSpectralData() const
{
	return m_impl->m_spectralData != nullptr;
}


icmm::ISpectralColorSpecification::ConstColorSpecPtr CPrinterProfile::GetSpectralSpecification() const
{
	return m_impl->m_spectralData;
}


void CPrinterProfile::SetSpectralSpecification(icmm::ISpectralColorSpecification::ConstColorSpecPtr spectralData)
{
	m_impl->m_spectralData = spectralData;
	
	// Recalculate gamut coverage
	if (spectralData) {
		m_impl->m_gamutCoverage = DEFAULT_GAMUT_COVERAGE; // Placeholder
	}
}


double CPrinterProfile::GetGamutCoverage() const
{
	return m_impl->m_gamutCoverage;
}


bool CPrinterProfile::IsValid() const
{
	return m_impl->m_spectralData != nullptr;
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
	
	xml.writeTextElement("PrinterName", m_impl->m_printerName);
	xml.writeTextElement("ColorSpace", QString::number(static_cast<int>(m_impl->m_colorSpace)));
	xml.writeTextElement("GamutCoverage", QString::number(m_impl->m_gamutCoverage));
	
	// Note: Spectral data serialization would require more complex handling
	// This is a simplified version
	xml.writeStartElement("SpectralData");
	xml.writeAttribute("hasData", m_impl->m_spectralData ? "true" : "false");
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
				m_impl->m_printerName = xml.readElementText();
			}
			else if (xml.name() == QStringLiteral("ColorSpace")) {
				int value = xml.readElementText().toInt();
				m_impl->m_colorSpace = static_cast<PrinterColorSpace>(value);
			}
			else if (xml.name() == QStringLiteral("GamutCoverage")) {
				m_impl->m_gamutCoverage = xml.readElementText().toDouble();
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


IDeviceToLabTransformation* CPrinterProfile::CreateDeviceToLabTransformation() const
{
	if (!IsValid()) {
		return nullptr;
	}
	
	return new CDeviceToLabTransformation(*this);
}


ILabToDeviceTransformation* CPrinterProfile::CreateLabToDeviceTransformation() const
{
	if (!IsValid()) {
		return nullptr;
	}
	
	return new CLabToDeviceTransformation(*this);
}


} // namespace iimgprint
