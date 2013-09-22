import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "imath"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iser" }

	Export{
		Depends{ name: "iser" }
	}
}
