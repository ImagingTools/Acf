import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ilog"

	Depends { name: "imod" }
	Depends { name: "iser" }
	Depends { name: "icomp" }

	Export {
		Depends { name: "imod" }
		Depends { name: "iser" }
		Depends { name: "icomp" }
	}
}
