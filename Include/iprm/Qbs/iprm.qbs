import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iprm"

	Depends { name: "iser" }

	Export {
		Depends { name: "iser" }
	}
}
