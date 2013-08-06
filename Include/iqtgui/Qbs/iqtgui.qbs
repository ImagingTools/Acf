import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct
//import "../../../Config/Qbs/QtRules.qbs" as QtRules

StaticProduct{
	name: "iqtgui"

	files: ["../*.h", "../*.cpp", "../*.ui", "../*.qrc"]

	Depends{ name: "iqt" }
	Depends{ name: "iprm" }
	Depends{ name: "ibase" }
	Depends{ name: "icomp" }
	Depends{ name: "Qt.gui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iqt" }
		Depends{ name: "iprm" }
		Depends{ name: "ibase" }
		Depends{ name: "icomp" }
		Depends{ name: "Qt.gui" }
		Depends{ name: "Qt.widgets" }
	}
}
