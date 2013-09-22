import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "i2d"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "imath" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "imath" }
		Depends{ name: "icomp" }
	}
}
