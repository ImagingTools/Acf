import "../../../Config/Qbs/ApplicationProduct.qbs" as ApplicationProduct

ApplicationProduct{
	name: "Arxc"

	files: ["../*.cpp"]

	Depends{ name: "ipackage" }
	Depends{ name: "iqt" }
}
