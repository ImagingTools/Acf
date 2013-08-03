import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ifilegui"

	Export {
		Depends { name: "ifile" }
		Depends { name: "iqtgui" }
	}
}
