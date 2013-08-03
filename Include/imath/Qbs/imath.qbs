import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "imath"

	Depends{ name: "iser" }

	Export {
		Depends{ name: "iser" }
	}
}
