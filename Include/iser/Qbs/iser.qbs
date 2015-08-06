import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "iser"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "istd" }

	Export{
		Depends{ name: "istd" }
	}
}
