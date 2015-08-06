import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "imod"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iser" }

	Export{
		Depends{ name: "iser" }
	}
}
