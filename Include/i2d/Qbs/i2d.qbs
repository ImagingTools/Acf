import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "i2d"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "imath" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "imath" }
		Depends{ name: "icomp" }
	}
}
