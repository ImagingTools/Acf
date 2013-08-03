import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqtprm"

	Depends { name: "iprm" }
	Depends { name: "iqtgui" }

	Export {
		Depends { name: "iprm" }
		Depends { name: "iqtgui" }
	}
}
