import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ipackage"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ifile" }
	Depends{ name: "iprm" }

	Export{
		Depends{ name: "ilog" }
		Depends{ name: "iprm" }
	}
}
