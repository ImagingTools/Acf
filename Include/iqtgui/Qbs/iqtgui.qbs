import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct
import "../../../Config/Qbs/QtRules.qbs" as QtRules

StaticProduct{
	name: "iqtgui"

	Group{
		name: "UI-Files"
		files: "../*.ui"
		fileTags: ["ui"]
	}

//	Depends{ name: "QtRules" }

	Export {
		Depends{ name: "icomp" }
		Depends{ name: "ibase" }
		Depends{ name: "iprm" }
		Depends{ name: "iqt" }
		Depends{ name: "Qt.gui" }
	}
}
