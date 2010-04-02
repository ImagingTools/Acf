#include "icomp/export.h"

#include "BasePck.h"


namespace BasePck
{


using namespace icomp;


I_EXPORT_DEFAULT_SERVICES;

I_EXPORT_PACKAGE(
			"Base",
			"Base system-undependent general package",
			II_META_DESCRIPTION(I_PACKAGE, "Undependend Standard Base"));

I_EXPORT_COMPONENT(
			XmlFileSerializer,
			"Simple file loader serializing objects using XML format",
			II_META_DESCRIPTION(I_DATA_PERSISTENCE, "File Loader Load Save Serializer Archive XML Format"));

I_EXPORT_COMPONENT(
			BinaryFileSerializer,
			"Simple file loader serializing objects using binary files",
			II_META_DESCRIPTION(I_DATA_PERSISTENCE, "Binary File Loader Load Save Serializer Archive Format"));

I_EXPORT_COMPONENT(
			ComposedLoader,
			"Composed loader delegating loading and saving to slave loaders",
			II_META_DESCRIPTION(I_DATA_PERSISTENCE, "Composed File Loader Load Save Slave Delegating"));

I_EXPORT_COMPONENT(
			TextFileSerializer,
			"Simple file serializer for text files",
			II_META_DESCRIPTION(I_DATA_PERSISTENCE, "Text File Serializer Format"));

I_EXPORT_COMPONENT(
			Instantiator,
			"Access specified components forcing to create its instances",
			II_META_DESCRIPTION(I_SERVICE, "Instance Init Create Free Components"));

I_EXPORT_COMPONENT(
			ApplicationInfo,
			"Simple application info",
			II_META_DESCRIPTION(I_SERVICE, "Application Info Version"));

I_EXPORT_COMPONENT(
			RegistryCodeSaver,
			"Stores registry as C++ code",
			II_META_DESCRIPTION(I_DATA_PERSISTENCE, "Code C++ File Loader Store Save Registry Compile"));

I_EXPORT_COMPONENT(
			ModelProxy,
			"Model proxy",
			II_META_DESCRIPTION(I_DATA_MODEL, "Model Proxy I_IMAGINGTOOLS"));

I_EXPORT_COMPONENT(
			ModelBinder,
			"Model binder",
			II_META_DESCRIPTION(I_DATA_MODEL, "Model Observer Bind Binder"));

I_EXPORT_COMPONENT(
			FilePersistence,
			"Allows object persistence using files",
			II_META_DESCRIPTION(I_DATA_PERSISTENCE, "Persistence File Free Create Store Restore Load Save"));

I_EXPORT_COMPONENT(
			ConsoleLog,
			"Simple log on windows console",
			II_META_DESCRIPTION(I_SERVICE, "Log Windows Message Consumer"));

I_EXPORT_COMPONENT(
			TextFileLog,
			"Simple text file log",
			II_META_DESCRIPTION(I_SERVICE, "Log Message Consumer"));

I_EXPORT_COMPONENT(
			FileCopyOverLoader,
			"General file copy or transform using loaders and any object",
			II_META_DESCRIPTION(I_DATA_PROCESSING, "File Copy Compile Transform Loader Saver"));

I_EXPORT_COMPONENT(
			CopyApp,
			"Copy application transforming files",
			II_META_DESCRIPTION(I_DATA_PROCESSING" "I_APPLICATION, "File Copy Compile Transform Application"));

I_EXPORT_COMPONENT(
			Uuid,
			"Unique identifier",
			II_META_DESCRIPTION(I_SERVICE, "Unique Identifier UUID Generated"));

I_EXPORT_COMPONENT(
			ObjectQueue,
			"Queue of any objects",
			II_META_DESCRIPTION(I_SERVICE, "Objects Queue List FIFO Stack Elements Serial Delay Line"));

I_EXPORT_COMPONENT(
			SelectableParamsSet,
			"Composed set of parameters",
			II_META_DESCRIPTION(I_DATA_PERSISTENCE, "Model Parameters Set Selection Options List"));

I_EXPORT_COMPONENT(
			ParamsManager,
			"Manager of parameters set",
			II_META_DESCRIPTION(I_DATA_PERSISTENCE, "Model Parameters Set Manager"));

I_EXPORT_COMPONENT(
			ComposedParamsSet,
			"Composed set of parameters",
			II_META_DESCRIPTION(I_DATA_MODEL, "Parameters Set List Composition Composed"));

I_EXPORT_COMPONENT(
			FileNameParam,
			"Parameters for file/directory selection",
			II_META_DESCRIPTION(I_DATA_MODEL, "File Directory Parameters URL Model"));

I_EXPORT_COMPONENT(
			EnableableParam,
			"Parameter with boolean flag",
			II_META_DESCRIPTION(I_DATA_MODEL, "Enabled Bool Parameter Model"));

I_EXPORT_COMPONENT(
			SingleDocumentTemplate,
			"Single document template for only one type of documents",
			II_META_DESCRIPTION(I_APPLICATION_LOGIC, "Single Document Template MVC"));

I_EXPORT_COMPONENT(
			CompositeDocumentTemplate,
			"Composite document template supporting more type of documents.It delegates document functionality to set of slave templates",
			II_META_DESCRIPTION(I_APPLICATION_LOGIC, "Composite Document Template MVC"));

I_EXPORT_COMPONENT(
			DocumentManagerListener,
			"Document workspace controller",
			II_META_DESCRIPTION(I_APPLICATION_LOGIC, "Model Proxy Composite Document Template MVC"));

I_EXPORT_COMPONENT(
			ComposedAuthorizationVerifier,
			"Allows to combine many authorization versifiers.",
			II_META_DESCRIPTION(I_APPLICATION_LOGIC, "Authorization Verifier Composition Right Security"));

I_EXPORT_COMPONENT(
			FixedAuthorisationVerifier,
			"Authorization verifier with fixed rights",
			II_META_DESCRIPTION(I_APPLICATION_LOGIC, "Simple Authorization Verifier Fixed Right Security"));

I_EXPORT_COMPONENT(
			StaticUserLogin,
			"Authorization verifier delegating authorisation request depent on logged user",
			II_META_DESCRIPTION(I_APPLICATION_LOGIC, "User Login Authorization Verifier Right Security"));

I_EXPORT_COMPONENT(
			Line,
			"Line 2D object",
			II_META_DESCRIPTION(I_DATA_MODEL, "Line 2D Object Parameter"));

I_EXPORT_COMPONENT(
			Position2d,
			"Position 2D object",
			II_META_DESCRIPTION(I_DATA_MODEL, "Position 2D Object Parameter"));

I_EXPORT_COMPONENT(
			Circle,
			"Circle 2D object",
			II_META_DESCRIPTION(I_DATA_MODEL, "Circle Position Radius 2D Object Parameter"));

I_EXPORT_COMPONENT(
			Rectangle,
			"Rectangle (orthogonal to axis) object",
			II_META_DESCRIPTION(I_DATA_MODEL, "Rectangle Orthogonal 2D Object Parameter"));

I_EXPORT_COMPONENT(
			Annulus,
			"Annulus object",
			II_META_DESCRIPTION(I_DATA_MODEL, "Annulus 2D Object Parameter"));

I_EXPORT_COMPONENT(
			AnnulusSegment,
			"Segment of annulus object",
			II_META_DESCRIPTION(I_DATA_MODEL, "Annulus Segment 2D Object Parameter"));

I_EXPORT_COMPONENT(
			Quadrangle,
			"Quadrangle object",
			II_META_DESCRIPTION(I_DATA_MODEL, "Quadrangle 2D Object Parameter"));

I_EXPORT_COMPONENT(
			AttributedRectangle,
			"Rectangle component with attributes",
			II_META_DESCRIPTION(I_DATA_MODEL, "Rectangle 2D Object Parameter"));

I_EXPORT_COMPONENT(
			TextDocument,
			"Simple text document",
			II_META_DESCRIPTION(I_DATA_MODEL, "Text Document"));

I_EXPORT_COMPONENT(
			SampledFunction2d,
			"Sampled function with 2-dimensional input",
			II_META_DESCRIPTION(I_DATA_MODEL, "Function Sample"));

I_EXPORT_COMPONENT(
			Histogram,
			"Image histogram",
			II_META_DESCRIPTION(I_DATA_MODEL, "Image Histogram"));

I_EXPORT_COMPONENT(
			TestIdManager,
			"Implementation of ID manager generating ID's for test",
			II_META_DESCRIPTION(I_DATA_PROCESSING, "ID Manager Test Supplier"));

I_EXPORT_COMPONENT(
			CascadedProcessor,
			"Sequence processing using slave processors",
			II_META_DESCRIPTION(I_DATA_PROCESSING, "Processor Sequence Buffer Cascade"));

I_EXPORT_COMPONENT(
			IterativeProcessor,
			"Iterative Processor",
			II_META_DESCRIPTION(I_DATA_PROCESSING, "Iterative Loop Sequence Processing"));

I_EXPORT_COMPONENT(
			IterativeProcessorParams,
			"Iterative Processor Params",
			II_META_DESCRIPTION(I_DATA_PROCESSING" "I_DATA_MODEL, "Iterative Filter Processing Parameter"));


} // namespace BasePck


