#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>


class CIChangeableTest: public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();

	void CIlluminantTest();
	void CTristimulusSpecificationTest();
	void CSpectrumInfoTest();
	void CRgbColorModelTest();
	void CCieLabColorModelTest();
	void CSpectralColorModelTest();
	void CSpectralColorSpecificationBaseTest();
	void CSubstractiveColorModelBaseTest();
	void CCmyColorModelTest();
	void CCmykColorModelTest();

	void cleanupTestCase();
};


