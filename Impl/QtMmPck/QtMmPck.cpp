#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QtMmPck.h"


namespace QtMmPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("QtMm", "Qt multimedia package", "Qt Multimedia");

I_EXPORT_COMPONENT(VideoView, "Video widget", "Qt Widget GUI Layout Video Multimedia");


} // namespace QtMmPck


