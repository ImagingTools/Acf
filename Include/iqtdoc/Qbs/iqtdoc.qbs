import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "iqtdoc"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtgui" }
	Depends{ name: "idoc" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.gui" }

	Export{
		Depends{ name: "iqtgui" }
		Depends{ name: "idoc" }
		Depends{ name: "Qt.gui" }
		Depends{ name: "Qt.widgets" }
	}
}
