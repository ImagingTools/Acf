import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "istd"

	files: ["../*.h", "../*.cpp"]
}
