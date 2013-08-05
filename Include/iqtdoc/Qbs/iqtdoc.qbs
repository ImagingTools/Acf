import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqtdoc"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.gui" }

	Export{
		Depends{ name: "idoc" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.gui" }
	}
}
