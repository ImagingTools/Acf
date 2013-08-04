import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iimg"

	Depends { name: "icomp" }
	Depends { name: "idoc" }
	Depends { name: "i2d" }
	Depends { name: "icmm" }
	Depends { name: "Qt.gui" }

	Export {
		Depends { name: "icomp" }
		Depends { name: "idoc" }
		Depends { name: "i2d" }
		Depends { name: "iqt" }
		Depends { name: "icmm" }
		Depends { name: "Qt.gui" }
	}
}
