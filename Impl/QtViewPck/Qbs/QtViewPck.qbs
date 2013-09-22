import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "QtViewPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iqt2d" }
	Depends{ name: "iview" }
	Depends{ name: "iqtgui" }

	Export{
		Depends{ name: "iqt2d" }
		Depends{ name: "iview" }
		Depends{ name: "iqtgui" }
	}
}
