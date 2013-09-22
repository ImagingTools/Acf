import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "icmm"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "imath" }
	Depends{ name: "iser" }

	Export{
		Depends{ name: "imath" }
		Depends{ name: "iser" }
	}
}
