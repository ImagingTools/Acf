import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ifilegui"

	Depends { name: "ifile" }
	Depends { name: "iqtgui" }

	Export {
		Depends { name: "ifile" }
		Depends { name: "iqtgui" }
	}
}
