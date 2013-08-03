import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "imod"

	Depends { name: "iser" }

	Export {
		Depends { name: "iser" }
	}
}
