import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "iqt"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "i2d" }
	Depends{ name: "icomp" }
	Depends{ name: "Qt.xml" }

	Export{
		Depends{ name: "i2d" }
		Depends{ name: "icomp" }
		Depends{ name: "Qt.xml" }
	}
}
