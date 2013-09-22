import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "iloggui"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtgui" }
	Depends{ name: "ilog" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.gui" }

	Export{
		Depends{ name: "iqtgui" }
		Depends{ name: "ilog" }
		Depends{ name: "Qt.widgets" }
		Depends{ name: "Qt.gui" }
	}
}
