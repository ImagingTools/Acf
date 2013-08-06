import qbs.base 1.0

Project{
	name: "Acf"

	property path acfDir: "../.."

	moduleSearchPaths: "qbs"

	references: [
		"Static.qbs",
		"Component.qbs",
		"Tool.qbs"
	]
}
