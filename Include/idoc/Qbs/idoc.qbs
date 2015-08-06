import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "idoc"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "imod" }
	Depends{ name: "iser" }

	Export{
		Depends{ name: "imod" }
		Depends{ name: "iser" }
	}
}
