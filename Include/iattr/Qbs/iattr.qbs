import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "iattr"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iser" }

	Export{
		Depends{ name: "iser" }
	}
}
