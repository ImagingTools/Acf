import qbs 1.0

Module{
	Depends{ name: 'ibase' }
	Depends{ name: 'icomp' }
	Depends{ name: 'istd' }
	Depends{ name: 'imod' }
	Depends{ name: 'iimg' }
	Depends{ name: 'ifile' }
	Depends{ name: 'iqtprm' }
	Depends{ name: 'iview' }
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
	Depends{ name: 'iloggui' }
	Depends{ name: 'ilog' }
	Depends{ name: 'iqt2d' }
	Depends{ name: 'imath' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }

	readonly property path componentFilePath: path + '/../../../.././QtGuiPck.arp'

	cpp.includePaths: [
		path + '/../../../../../Impl',
		path + '/../../../../../Include'
	]
}
