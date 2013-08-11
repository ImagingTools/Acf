import qbs.base 1.0
// Standard settings for an ACF static library

StaticLibrary{
	destinationDirectory: "Lib"

	Depends{ name: "Qt.core" }
	Depends{ name: "cpp" }

	cpp.defines: ['I_QBS']
	cpp.includePaths: ["../..", product.buildDirectory]

	Export{
		Depends{ name: "cpp" }
		Depends{ name: "Qt.core" }

		cpp.defines: ['I_QBS']
		cpp.includePaths: ["../..", product.buildDirectory]
	}
}
