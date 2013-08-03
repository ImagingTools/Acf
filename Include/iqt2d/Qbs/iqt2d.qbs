import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqt2d"

	Depends { name: "i2d" }
	Depends { name: "iview" }

	Export {
		Depends { name: "i2d" }
		Depends { name: "iview" }
	}
}
