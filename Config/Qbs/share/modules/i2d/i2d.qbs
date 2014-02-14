import qbs 1.0

Module{
	Depends{ name: 'imath' }
	Depends{ name: 'iser' }
	Depends{ name: 'imod' }
	Depends{ name: 'istd' }
	Depends{ name: 'icomp' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/i2d.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
