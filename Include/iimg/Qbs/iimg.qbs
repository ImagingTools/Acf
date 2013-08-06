import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iimg"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "i2d" }
	Depends{ name: "icmm" }
	Depends{ name: "icomp" }
	Depends{ name: "Qt.gui" }

	Export{
		Depends{ name: "idoc" }
		Depends{ name: "i2d" }
		Depends{ name: "iqt" }
		Depends{ name: "icmm" }
		Depends{ name: "icomp" }
		Depends{ name: "Qt.gui" }
	}
}
