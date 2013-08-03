import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iser"

	Depends { name: "istd" }

	Export {
		Depends { name: "istd" }
	}
}
