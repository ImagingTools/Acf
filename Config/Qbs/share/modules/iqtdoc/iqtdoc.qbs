import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'imath' }
	Depends{ name: 'iqtgui' }
	Depends{ name: 'imod' }
	Depends{ name: 'iprm' }
	Depends{ name: 'ibase' }
	Depends{ name: 'ifile' }
	Depends{ name: 'ilog' }
	Depends{ name: 'istd' }
	Depends{ name: 'iqt' }
	Depends{ name: 'icomp' }
	Depends{ name: 'i2d' }
	Depends{ name: 'idoc' }
	Depends{ name: 'ifilegui' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.widgets' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'Qt.core' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/iqtdoc.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
