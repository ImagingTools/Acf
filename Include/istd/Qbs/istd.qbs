import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "istd"

	files: ["../*.h", "../*.cpp"]

    Depends{ name: "acf" }

    Export{
        Depends{ name: "acf" }
    }
}
