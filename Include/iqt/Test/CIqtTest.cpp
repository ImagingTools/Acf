// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CIqtTest.h"


// protected slots

void CIqtTest::initTestCase()
{
}


void CIqtTest::TestQSizeConversions()
{
	// Test CIndex2d to QSize conversion
	istd::CIndex2d size1(800, 600);
	QSize qsize = iqt::GetQSize(size1);
	QCOMPARE(qsize.width(), 800);
	QCOMPARE(qsize.height(), 600);

	// Test QSize to CIndex2d conversion
	QSize qsize2(1024, 768);
	istd::CIndex2d size2 = iqt::GetCIndex2d(qsize2);
	QCOMPARE(size2.GetX(), 1024);
	QCOMPARE(size2.GetY(), 768);

	// Test round-trip conversion
	istd::CIndex2d size3(640, 480);
	istd::CIndex2d size3RoundTrip = iqt::GetCIndex2d(iqt::GetQSize(size3));
	QCOMPARE(size3, size3RoundTrip);
}


void CIqtTest::TestQPointConversions()
{
	// Test CIndex2d to QPoint conversion
	istd::CIndex2d pos1(100, 200);
	QPoint qpoint = iqt::GetQPoint(pos1);
	QCOMPARE(qpoint.x(), 100);
	QCOMPARE(qpoint.y(), 200);

	// Test QPoint to CIndex2d conversion
	QPoint qpoint2(150, 250);
	istd::CIndex2d pos2 = iqt::GetCIndex2d(qpoint2);
	QCOMPARE(pos2.GetX(), 150);
	QCOMPARE(pos2.GetY(), 250);

	// Test CVector2d to QPointF conversion
	i2d::CVector2d vec(3.14, 2.71);
	QPointF qpointf = iqt::GetQPoint(vec);
	QVERIFY(qAbs(qpointf.x() - 3.14) < 0.001);
	QVERIFY(qAbs(qpointf.y() - 2.71) < 0.001);

	// Test round-trip conversion for CIndex2d
	istd::CIndex2d pos3(50, 75);
	istd::CIndex2d pos3RoundTrip = iqt::GetCIndex2d(iqt::GetQPoint(pos3));
	QCOMPARE(pos3, pos3RoundTrip);
}


void CIqtTest::TestQRectFConversions()
{
	// Test CRectangle to QRectF conversion
	i2d::CRectangle rect1(10.0, 20.0, 100.0, 50.0);
	QRectF qrectf = iqt::GetQRectF(rect1);
	QVERIFY(qAbs(qrectf.left() - 10.0) < 0.001);
	QVERIFY(qAbs(qrectf.top() - 20.0) < 0.001);
	QVERIFY(qAbs(qrectf.width() - 100.0) < 0.001);
	QVERIFY(qAbs(qrectf.height() - 50.0) < 0.001);

	// Test QRectF to CRectangle conversion
	QRectF qrectf2(5.0, 15.0, 200.0, 100.0);
	i2d::CRectangle rect2 = iqt::GetCRectangle(qrectf2);
	QVERIFY(qAbs(rect2.GetLeft() - 5.0) < 0.001);
	QVERIFY(qAbs(rect2.GetTop() - 15.0) < 0.001);
	QVERIFY(qAbs(rect2.GetWidth() - 200.0) < 0.001);
	QVERIFY(qAbs(rect2.GetHeight() - 100.0) < 0.001);

	// Test round-trip conversion
	i2d::CRectangle rect3(1.0, 2.0, 3.0, 4.0);
	i2d::CRectangle rect3RoundTrip = iqt::GetCRectangle(iqt::GetQRectF(rect3));
	QVERIFY(qAbs(rect3.GetLeft() - rect3RoundTrip.GetLeft()) < 0.001);
	QVERIFY(qAbs(rect3.GetTop() - rect3RoundTrip.GetTop()) < 0.001);
	QVERIFY(qAbs(rect3.GetWidth() - rect3RoundTrip.GetWidth()) < 0.001);
	QVERIFY(qAbs(rect3.GetHeight() - rect3RoundTrip.GetHeight()) < 0.001);
}


void CIqtTest::TestQLineConversions()
{
	// Test CLine2d to QLine conversion
	i2d::CLine2d line1(10.0, 20.0, 100.0, 50.0);
	QLine qline = iqt::GetQLine(line1);
	QCOMPARE(qline.x1(), 10);
	QCOMPARE(qline.y1(), 20);
	QCOMPARE(qline.x2(), 100);
	QCOMPARE(qline.y2(), 50);

	// Test QLine to CLine2d conversion
	QLine qline2(5, 15, 200, 100);
	i2d::CLine2d line2 = iqt::GetCLine2d(qline2);
	QVERIFY(qAbs(line2.GetPoint1().GetX() - 5.0) < 0.001);
	QVERIFY(qAbs(line2.GetPoint1().GetY() - 15.0) < 0.001);
	QVERIFY(qAbs(line2.GetPoint2().GetX() - 200.0) < 0.001);
	QVERIFY(qAbs(line2.GetPoint2().GetY() - 100.0) < 0.001);
}


void CIqtTest::TestQLineFConversions()
{
	// Test CLine2d to QLineF conversion
	i2d::CLine2d line1(10.5, 20.5, 100.5, 50.5);
	QLineF qlinef = iqt::GetQLineF(line1);
	QVERIFY(qAbs(qlinef.x1() - 10.5) < 0.001);
	QVERIFY(qAbs(qlinef.y1() - 20.5) < 0.001);
	QVERIFY(qAbs(qlinef.x2() - 100.5) < 0.001);
	QVERIFY(qAbs(qlinef.y2() - 50.5) < 0.001);

	// Test QLineF to CLine2d conversion
	QLineF qlinef2(5.5, 15.5, 200.5, 100.5);
	i2d::CLine2d line2 = iqt::GetCLine2d(qlinef2);
	QVERIFY(qAbs(line2.GetPoint1().GetX() - 5.5) < 0.001);
	QVERIFY(qAbs(line2.GetPoint1().GetY() - 15.5) < 0.001);
	QVERIFY(qAbs(line2.GetPoint2().GetX() - 200.5) < 0.001);
	QVERIFY(qAbs(line2.GetPoint2().GetY() - 100.5) < 0.001);

	// Test round-trip conversion
	i2d::CLine2d line3(1.1, 2.2, 3.3, 4.4);
	i2d::CLine2d line3RoundTrip = iqt::GetCLine2d(iqt::GetQLineF(line3));
	QVERIFY(qAbs(line3.GetPoint1().GetX() - line3RoundTrip.GetPoint1().GetX()) < 0.001);
	QVERIFY(qAbs(line3.GetPoint1().GetY() - line3RoundTrip.GetPoint1().GetY()) < 0.001);
	QVERIFY(qAbs(line3.GetPoint2().GetX() - line3RoundTrip.GetPoint2().GetX()) < 0.001);
	QVERIFY(qAbs(line3.GetPoint2().GetY() - line3RoundTrip.GetPoint2().GetY()) < 0.001);
}


void CIqtTest::TestQRectConversions()
{
	// Test CRect to QRect conversion
	i2d::CRect rect1(10, 20, 100, 50);
	QRect qrect = iqt::GetQRect(rect1);
	QCOMPARE(qrect.left(), 10);
	QCOMPARE(qrect.top(), 20);
	QCOMPARE(qrect.width(), 90);
	QCOMPARE(qrect.height(), 30);

	// Test QRect to CRect conversion
	QRect qrect2(5, 15, 200, 100);
	i2d::CRect rect2 = iqt::GetCRect(qrect2);
	QCOMPARE(rect2.GetLeft(), 5);
	QCOMPARE(rect2.GetTop(), 15);
	// Note: CRect uses right/bottom as exclusive bounds
	QCOMPARE(rect2.GetRight(), 206);
	QCOMPARE(rect2.GetBottom(), 116);

	// Test round-trip conversion
	i2d::CRect rect3(1, 2, 10, 10);
	i2d::CRect rect3RoundTrip = iqt::GetCRect(iqt::GetQRect(rect3));
	QCOMPARE(rect3.GetLeft(), rect3RoundTrip.GetLeft());
	QCOMPARE(rect3.GetTop(), rect3RoundTrip.GetTop());
	QCOMPARE(rect3.GetWidth(), rect3RoundTrip.GetWidth());
	QCOMPARE(rect3.GetHeight(), rect3RoundTrip.GetHeight());
}


void CIqtTest::cleanupTestCase()
{
}


I_ADD_TEST(CIqtTest);
