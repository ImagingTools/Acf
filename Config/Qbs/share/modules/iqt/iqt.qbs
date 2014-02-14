import qbs 1.0

Module{
	Depends{ name: 'imath' }
	Depends{ name: 'iser' }
	Depends{ name: 'imod' }
	Depends{ name: 'istd' }
	Depends{ name: 'i2d' }
	Depends{ name: 'icomp' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'Qt.xml' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/iqt.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
