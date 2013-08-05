import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqtprm"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iprm" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.gui" }

	Export{
		Depends{ name: "iprm" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.gui" }
	}
}
