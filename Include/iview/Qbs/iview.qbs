import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iview"

	Depends { name: "iqt2d" }
	Depends { name: "iqtgui" }

	Export {
		Depends { name: "iqt2d" }
		Depends { name: "iqtgui" }
	}
}
