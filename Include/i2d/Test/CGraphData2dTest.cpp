#include <i2d/Test/CGraphData2dTest.h>


#include <i2d/CGraphData2d.h>
#include <itest/CStandardTestExecutor.h>


void CGraphData2dTest::initTestCase()
{
	// Setup test environment if needed
}


void CGraphData2dTest::testConstruction()
{
	i2d::CGraphData2d graph;
	
	QCOMPARE(graph.GetCurvesCount(), 0);
	QVERIFY(graph.GetTitle().isEmpty());
	QVERIFY(graph.GetXAxisLabel().isEmpty());
	QVERIFY(graph.GetYAxisLabel().isEmpty());
	QVERIFY(graph.IsLegendVisible());
	QVERIFY(graph.IsGridVisible());
}


void CGraphData2dTest::testAddCurve()
{
	i2d::CGraphData2d graph;
	
	i2d::CGraphData2d::Curve curve1;
	curve1.name = "Test Curve 1";
	curve1.color = Qt::red;
	curve1.points.append(i2d::CVector2d(0.0, 0.0));
	curve1.points.append(i2d::CVector2d(1.0, 1.0));
	curve1.points.append(i2d::CVector2d(2.0, 4.0));
	
	graph.AddCurve(curve1);
	
	QCOMPARE(graph.GetCurvesCount(), 1);
	QCOMPARE(graph.GetCurve(0).name, QString("Test Curve 1"));
	QCOMPARE(graph.GetCurve(0).color, QColor(Qt::red));
	QCOMPARE(graph.GetCurve(0).points.count(), 3);
	
	i2d::CGraphData2d::Curve curve2;
	curve2.name = "Test Curve 2";
	curve2.color = Qt::blue;
	curve2.points.append(i2d::CVector2d(0.0, 1.0));
	curve2.points.append(i2d::CVector2d(1.0, 2.0));
	
	graph.AddCurve(curve2);
	
	QCOMPARE(graph.GetCurvesCount(), 2);
	QCOMPARE(graph.GetCurve(1).name, QString("Test Curve 2"));
}


void CGraphData2dTest::testRemoveCurve()
{
	i2d::CGraphData2d graph;
	
	i2d::CGraphData2d::Curve curve1;
	curve1.name = "Curve 1";
	graph.AddCurve(curve1);
	
	i2d::CGraphData2d::Curve curve2;
	curve2.name = "Curve 2";
	graph.AddCurve(curve2);
	
	QCOMPARE(graph.GetCurvesCount(), 2);
	
	graph.RemoveCurve(0);
	
	QCOMPARE(graph.GetCurvesCount(), 1);
	QCOMPARE(graph.GetCurve(0).name, QString("Curve 2"));
}


void CGraphData2dTest::testClearCurves()
{
	i2d::CGraphData2d graph;
	
	i2d::CGraphData2d::Curve curve;
	curve.name = "Test";
	graph.AddCurve(curve);
	graph.AddCurve(curve);
	graph.AddCurve(curve);
	
	QCOMPARE(graph.GetCurvesCount(), 3);
	
	graph.ClearCurves();
	
	QCOMPARE(graph.GetCurvesCount(), 0);
}


void CGraphData2dTest::testAxisLabels()
{
	i2d::CGraphData2d graph;
	
	graph.SetXAxisLabel("Time (s)");
	QCOMPARE(graph.GetXAxisLabel(), QString("Time (s)"));
	
	graph.SetYAxisLabel("Amplitude (V)");
	QCOMPARE(graph.GetYAxisLabel(), QString("Amplitude (V)"));
	
	graph.SetTitle("Test Graph");
	QCOMPARE(graph.GetTitle(), QString("Test Graph"));
}


void CGraphData2dTest::testAxisRanges()
{
	//i2d::CGraphData2d graph;
	//
	//// Test auto-calculated ranges with no data
	//istd::CRange xRange = graph.GetXAxisRange();
	//istd::CRange yRange = graph.GetYAxisRange();
	//QVERIFY(xRange.IsValid());
	//QVERIFY(yRange.IsValid());
	//
	//// Add some data
	//i2d::CGraphData2d::Curve curve;
	//curve.points.append(i2d::CVector2d(0.0, 0.0));
	//curve.points.append(i2d::CVector2d(10.0, 20.0));
	//graph.AddCurve(curve);
	//
	//// Test auto-calculated ranges with data
	//xRange = graph.GetXAxisRange();
	//yRange = graph.GetYAxisRange();
	//QVERIFY(xRange.IsValid());
	//QVERIFY(yRange.IsValid());
	//QVERIFY(xRange.GetMinValue() <= 0.0);
	//QVERIFY(xRange.GetMaxValue() >= 10.0);
	//QVERIFY(yRange.GetMinValue() <= 0.0);
	//QVERIFY(yRange.GetMaxValue() >= 20.0);
	//
	//// Test manual ranges
	//graph.SetXAxisRange(istd::CRange(-5.0, 15.0));
	//graph.SetYAxisRange(istd::CRange(-10.0, 30.0));
	//
	//xRange = graph.GetXAxisRange();
	//yRange = graph.GetYAxisRange();
	//
	//QCOMPARE(xRange.GetMinValue(), -5.0);
	//QCOMPARE(xRange.GetMaxValue(), 15.0);
	//QCOMPARE(yRange.GetMinValue(), -10.0);
	//QCOMPARE(yRange.GetMaxValue(), 30.0);
}


void CGraphData2dTest::testLegendVisibility()
{
	i2d::CGraphData2d graph;
	
	QVERIFY(graph.IsLegendVisible());
	
	graph.SetLegendVisible(false);
	QVERIFY(!graph.IsLegendVisible());
	
	graph.SetLegendVisible(true);
	QVERIFY(graph.IsLegendVisible());
}


void CGraphData2dTest::testGridVisibility()
{
	i2d::CGraphData2d graph;
	
	QVERIFY(graph.IsGridVisible());
	
	graph.SetGridVisible(false);
	QVERIFY(!graph.IsGridVisible());
	
	graph.SetGridVisible(true);
	QVERIFY(graph.IsGridVisible());
}


void CGraphData2dTest::testBoundingBox()
{
//	i2d::CGraphData2d graph;
//	
//	// Test with no data
//	i2d::CRectangle bbox = graph.GetBoundingBox();
//	QVERIFY(bbox.IsValid());
//	
//	// Add data
//	i2d::CGraphData2d::Curve curve;
//	curve.points.append(i2d::CVector2d(1.0, 2.0));
//	curve.points.append(i2d::CVector2d(5.0, 8.0));
//	graph.AddCurve(curve);
//	
//	bbox = graph.GetBoundingBox();
//	QVERIFY(bbox.IsValid());
//	QVERIFY(bbox.GetLeft() <= 1.0);
//	QVERIFY(bbox.GetRight() >= 5.0);
//	QVERIFY(bbox.GetTop() <= 2.0);
//	QVERIFY(bbox.GetBottom() >= 8.0);
}


void CGraphData2dTest::cleanupTestCase()
{
	// Cleanup test environment if needed
}


I_ADD_TEST(CGraphData2dTest);
