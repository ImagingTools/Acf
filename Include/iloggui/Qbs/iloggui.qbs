import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iloggui"

	Depends { name: "ilog" }
	Depends { name: "iqtgui" }

	Export {
		Depends { name: "ilog" }
		Depends { name: "iqtgui" }
	}
}
