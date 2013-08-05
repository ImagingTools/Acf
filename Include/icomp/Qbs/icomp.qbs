import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "icomp"

	files: ["../*.h", "../*.cpp"]

	Export{
		Depends{ name: "imod" }
	}
}
