import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ipackage"

	Depends { name: "ilog" }
	Depends { name: "ifile" }
	Depends { name: "iprm" }

	Export {
		Depends { name: "ilog" }
		Depends { name: "ifile" }
		Depends { name: "iprm" }
	}
}
