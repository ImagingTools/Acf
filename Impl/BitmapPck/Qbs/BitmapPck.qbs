import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "BitmapPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iimg" }
	Export{
		Depends{ name: "iimg" }
	}
}
