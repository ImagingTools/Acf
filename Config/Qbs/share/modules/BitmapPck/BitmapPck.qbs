import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'imath' }
	Depends{ name: 'imod' }
	Depends{ name: 'iprm' }
	Depends{ name: 'ibase' }
	Depends{ name: 'ilog' }
	Depends{ name: 'istd' }
	Depends{ name: 'ipackage' }
	Depends{ name: 'i2d' }
	Depends{ name: 'iqt' }
	Depends{ name: 'icmm' }
	Depends{ name: 'icomp' }
	Depends{ name: 'iimg' }
	Depends{ name: 'idoc' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }

	readonly property path componentFilePath: path + '/../../../.././BitmapPck.arp'

	cpp.includePaths: [
		path + '/../../../../../Impl',
		path + '/../../../../../Include'
	]
}
