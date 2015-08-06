import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "iprm"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iser" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "iser" }
		Depends{ name: "icomp" }
	}
}
