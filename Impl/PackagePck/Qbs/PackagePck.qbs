import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "PackagePck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ilog" }
	Depends{ name: "ipackage" }
}
