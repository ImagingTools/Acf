import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "QtViewPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "Qt.gui" }
	Depends{ name: "iqtgui" }
	Depends{ name: "iview" }
}
