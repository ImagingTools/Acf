import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqt"

	Depends { name: "icomp" }
	Depends { name: "Qt.xml" }

	Export {
		Depends { name: "icomp" }
		Depends { name: "iimg" }
		Depends { name: "Qt.xml" }
	}
}
