import qbs.base 1.0
// Standard settings for an ACF application

Application{
	destinationDirectory: "Bin"

	Depends{ name: "Qt.core" }
	Depends{ name: "cpp" }

	cpp.defines: ['I_QBS']
}
