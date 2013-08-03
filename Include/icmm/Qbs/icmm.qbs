import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "icmm"

	Depends { name: "imath" }

	Export {
		Depends { name: "imath" }
	}
}
