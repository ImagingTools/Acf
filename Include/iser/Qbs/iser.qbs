import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "iser"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "istd" }

	Export{
		Depends{ name: "istd" }
	}
}
