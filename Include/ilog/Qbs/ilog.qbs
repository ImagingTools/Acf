import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ilog"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "imod" }
	Depends{ name: "iser" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "imod" }
		Depends{ name: "iser" }
		Depends{ name: "icomp" }
	}
}
