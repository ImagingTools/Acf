import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "imod"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iser" }

	Export{
		Depends{ name: "iser" }
	}
}
