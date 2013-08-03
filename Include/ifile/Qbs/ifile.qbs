import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ifile"

	Depends { name: "ibase" }
	Depends { name: "ilog" }
	Depends { name: "Qt.gui" }	// TODO: remove this dependency

	Export {
		Depends { name: "ibase" }
		Depends { name: "ilog" }
		Depends { name: "Qt.gui" }	// TODO: remove this dependency
	}
}
