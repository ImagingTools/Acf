import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ifilegui"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iqtgui" }

	Export{
		Depends{ name: "ifile" }
		Depends{ name: "iqtgui" }
	}
}
