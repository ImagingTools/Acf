import qbs.base 1.0
// Standard settings for an ACF application

Application{
	type: ["application", "acf_share"]

	Depends{ name: "acf" }
	Depends{ name: "Qt.core" }
	Depends{ name: "cpp" }

	cpp.defines: ['I_QBS']

	Group{
		fileTagsFilter: ["application"]
		qbs.install: true
		qbs.installDir: acf.targetBinSubdir
	}
}
