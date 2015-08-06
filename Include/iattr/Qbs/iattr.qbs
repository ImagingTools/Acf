import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "iattr"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iser" }
	Depends{ name: "imod" }

	Export{
		Depends{ name: "iser" }
		Depends{ name: "imod" }
	}
}
