import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "PackagePck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ilog" }
	Export{
		Depends{ name: "ilog" }
	}
}
