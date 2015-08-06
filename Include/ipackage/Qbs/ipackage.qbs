import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "ipackage"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ifile" }
	Depends{ name: "iprm" }

	Export{
		Depends{ name: "ilog" }
		Depends{ name: "iprm" }
	}
}
