import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "imath"

	files: ["../*.h", "../*.cpp"]

	Export{
		Depends{ name: "iser" }
	}
}
