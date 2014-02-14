import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'imath' }
	Depends{ name: 'imod' }
	Depends{ name: 'ibase' }
	Depends{ name: 'ilog' }
	Depends{ name: 'istd' }
	Depends{ name: 'i2d' }
	Depends{ name: 'iqt' }
	Depends{ name: 'icmm' }
	Depends{ name: 'icomp' }
	Depends{ name: 'idoc' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.core' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/iimg.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
