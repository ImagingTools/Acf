import qbs 1.0

Module{
	Depends{ name: 'imath' }
	Depends{ name: 'iser' }
	Depends{ name: 'iqtgui' }
	Depends{ name: 'imod' }
	Depends{ name: 'iprm' }
	Depends{ name: 'ibase' }
	Depends{ name: 'ilog' }
	Depends{ name: 'istd' }
	Depends{ name: 'i2d' }
	Depends{ name: 'icomp' }
	Depends{ name: 'iqt' }
	Depends{ name: 'icmm' }
	Depends{ name: 'iimg' }
	Depends{ name: 'idoc' }
	Depends{ name: 'iview' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.widgets' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'Qt.core' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/iqt2d.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
