import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "AcfLoc"

	Group{
		name: "Translations"
		files: ["*.ts"]
		prefix: "../"
		fileTags: ["qm"]
	}

	Group{
		name: "Resources"
		prefix: "../"
		files: ["*.qrc"]
		fileTags: ["cpp"]
	}
}
