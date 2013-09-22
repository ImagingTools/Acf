import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "FilePck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ifile" }
	Depends{ name: "ilog" }
	Export{
		Depends{ name: "ifile" }
		Depends{ name: "ilog" }
	}
}
