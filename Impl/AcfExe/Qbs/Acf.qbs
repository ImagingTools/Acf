import "../../../Config/Qbs/ApplicationProduct.qbs" as ApplicationProduct

ApplicationProduct{
	name: "Acf"

	Depends{ name: "icomp" }
	Depends{ name: "ipackage" }
	Depends{ name: "Qt.gui" }
}
