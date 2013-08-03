import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqtdoc"

	Depends { name: "idoc" }
	Depends { name: "iqtgui" }

	Export {
		Depends { name: "idoc" }
		Depends { name: "iqtgui" }
	}
}
