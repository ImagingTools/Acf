import qbs.base 1.0

Project{
	name: "AcfAll"
	id: acfProject

	property path acfDir: "../../"
	property path acfConfigDir: "../../Config/Qbs/"

	moduleSearchPaths: ["../../Config/Qbs"]

	references: [
		"Static.qbs",
		"Component.qbs",
		"Tool.qbs"
	]
}
