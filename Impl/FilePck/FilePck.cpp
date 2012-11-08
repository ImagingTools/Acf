#include "FilePck.h"


#include "icomp/export.h"


namespace FilePck
{


I_EXPORT_PACKAGE(
			"Acf/File",
			"Package for file system primitives management",
			IM_PROJECT("ACF") IM_TAG("File") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			FileNameParam,
			"Parameters for file/directory selection",
			"Name Directory Parameters URL" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Parameter"));

I_EXPORT_COMPONENT(
			RelativeFileNameParam,
			"Provide file name relative to some other optional directory",
			"Name Relative Directory Parameters URL" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Parameter"));

I_EXPORT_COMPONENT(
			SystemLocation,
			"System locations provider",
			"System Path File Location Temp Documents Media Cache" IM_TAG("Model"));

I_EXPORT_COMPONENT(
			GeneratedFileNameParam,
			"Provide auto generated file or directory name",
			"Name Relative Directory Parameters URL" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Parameter"));

I_EXPORT_COMPONENT(
			FileListProvider,
			"Provide list of file synchronized with some directory",
			"List Provider Directory");


} // namespace FilePck


