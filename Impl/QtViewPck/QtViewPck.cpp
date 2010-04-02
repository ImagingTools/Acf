#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QtViewPck.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


namespace QtViewPck
{


using namespace icomp;


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("Acf/Qt/Gui/View", "Standard Qt package", "Qt Standard");

I_EXPORT_COMPONENT(
			SceneConnector,
			"Display scane provider, scene extender and connect it",
			"Scene Connector Provider Extender Display Console GUI Qt");

I_EXPORT_COMPONENT(
			SceneProvider,
			"Standard scene provider",
			"Scene Provider Extender Display Console GUI Qt");

I_EXPORT_COMPONENT(
			ImageView,
			"Display bitmap using observer pattern",
			"Image Bitmap Display Viewer Observer Qt");

I_EXPORT_COMPONENT(
			Line2dParamsGui,
			"Line parameter GUI displaying line using scene",
			"Line Parameters Observer Display GUI Qt");

I_EXPORT_COMPONENT(
			RectangleParamsGui,
			"Rectangle parameter GUI displaying a rectangle using scene",
			"Rectangle Parameters Observer Display GUI Qt");

I_EXPORT_COMPONENT(
			CircleParamsGui,
			"Circle parameter GUI displaying a cicle using scene",
			"Circle Parameters Observer Display GUI Qt");

I_EXPORT_COMPONENT(
			AnnulusParamsGui,
			"Annulus parameter GUI displaying a annulus using scene",
			"Annulus Radius Radii Parameters Observer Display GUI Qt");

I_EXPORT_COMPONENT(
			AnnulusSegmentParamsGui,
			"Annulus segment parameter GUI displaying a annulus using scene",
			"Annulus Segment Angle Radius Radii Parameters Observer Display GUI Qt");

I_EXPORT_COMPONENT(
			QuadrangleParamsGui,
			"Quadrangle parameter GUI displaying a quadrangle object using scene",
			"Quadrangle Parameters Observer Display GUI Qt");

I_EXPORT_COMPONENT(
			SceneControllerGui,
			"Scene controller allowing to control scale and other scene properties",
			"Observer Display GUI Qt Scene Controller Zoom Rotate");


} // namespace QtViewPck


