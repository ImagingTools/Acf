import qbs 1.0

Module{
	Depends{ name: 'ibase' }
	Depends{ name: 'icomp' }
	Depends{ name: 'istd' }
	Depends{ name: 'imod' }
	Depends{ name: 'iimg' }
	Depends{ name: 'ifile' }
	Depends{ name: 'iqtgui' }
	Depends{ name: 'iser' }
	Depends{ name: 'ifilegui' }
	Depends{ name: 'iqt' }
	Depends{ name: 'i2d' }
	Depends{ name: 'idoc' }
	Depends{ name: 'iqtdoc' }
	Depends{ name: 'ipackage' }
	Depends{ name: 'iprm' }
	Depends{ name: 'icmm' }
	Depends{ name: 'ilog' }
	Depends{ name: 'imath' }
	Depends{ name: 'cpp' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.core' }

	readonly property path componentFilePath: path + '/../../../.././QtPck.arp'

	cpp.includePaths: [
		path + '/../../../../../Impl',
		path + '/../../../../../Include'
	]
}
