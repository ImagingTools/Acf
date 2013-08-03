import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "icomp"

	Depends { name: "imod" }

	Export {
		Depends { name: "imod" }
	}
}
