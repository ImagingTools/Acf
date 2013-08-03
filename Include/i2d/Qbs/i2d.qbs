import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "i2d"

	Depends{ name: "imath" }
	Depends{ name: "icomp" }

	Export {
		Depends{ name: "imath" }
		Depends{ name: "icomp" }
	}
}
