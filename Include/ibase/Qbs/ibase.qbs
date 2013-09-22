import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "ibase"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "icomp" }

	Export{
		Depends{ name: "ilog" }
		Depends{ name: "icomp" }
	}
}
