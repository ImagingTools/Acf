import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "icmm"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "imath" }
	Depends{ name: "iser" }

	Export{
		Depends{ name: "imath" }
		Depends{ name: "iser" }
	}
}
