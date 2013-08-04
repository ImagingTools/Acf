import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct
//import "../../../Config/Qbs/QtRules.qbs" as QtRules

StaticProduct{
	name: "iqtgui"

	Depends{ name: "icomp" }
	Depends{ name: "ibase" }
	Depends{ name: "iprm" }
	Depends{ name: "iqt" }
	Depends{ name: "Qt.gui" }

	files: ["../*.ui", "../*.cpp", "../*.h"]

	Export{
		Depends{ name: "icomp" }
		Depends{ name: "ibase" }
		Depends{ name: "iprm" }
		Depends{ name: "iqt" }
		Depends{ name: "Qt.gui" }
	}
}
