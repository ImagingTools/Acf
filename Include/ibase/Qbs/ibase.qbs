import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ibase"

	Depends { name: "ilog" }
	Depends { name: "icomp" }

	Export {
		Depends { name: "ilog" }
		Depends { name: "icomp" }
	}
}
