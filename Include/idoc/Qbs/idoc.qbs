import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "idoc"

	Depends { name: "imod" }
	Depends { name: "iser" }

	Export {
		Depends { name: "imod" }
		Depends { name: "iser" }
	}
}
