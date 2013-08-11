import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "AcfLoc"

	Group{
		name: "Translations"
		files: ["../*.ts"]
	}

	Group{
		name: "Resources"
		files: ["../*.qrc"]
		fileTags: ["qrc_generated"]
	}

	Depends{ name: "acf" }
	Qt.core.qmFilesDir: Qt.core.generatedFilesDir
}
