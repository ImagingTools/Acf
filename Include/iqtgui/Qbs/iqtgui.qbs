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

	Group{
		name: "UI-Files"
		files: "../*.ui"
		fileTags: ["ui"]
	}

	Export{
		Depends{ name: "icomp" }
		Depends{ name: "ibase" }
		Depends{ name: "iprm" }
		Depends{ name: "iqt" }
		Depends{ name: "Qt.gui" }
	}
}
