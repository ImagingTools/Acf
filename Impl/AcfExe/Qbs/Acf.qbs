import "../../../Config/Qbs/ApplicationProduct.qbs" as ApplicationProduct

ApplicationProduct{
	name: "Acf"

	files: ["../*.cpp"]

	Depends{ name: "icomp" }
	Depends{ name: "ipackage" }
	Depends{ name: "Qt.gui" }
}
