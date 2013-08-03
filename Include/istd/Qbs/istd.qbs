import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "istd"

	Depends { name: "Qt.core" }

	Export {
		Depends { name: "Qt.core" }
	}
}
