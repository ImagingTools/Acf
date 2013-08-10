import qbs.base 1.0
import qbs.FileInfo

Module{
	name: "acf"

	Depends{ name: "Qt.core" }
	Depends{ name: "cpp" }

	property string COMPILER_NAME: "Qbs"

	Properties{
		condition: cpp.compilerPath.contains("2005") || cpp.compilerPath.contains("VC8")
		COMPILER_NAME: "VC10"
	}
	Properties{
		condition: cpp.compilerPath.contains("2008") || cpp.compilerPath.contains("VC9")
		COMPILER_NAME: "VC10"
	}
	Properties{
		condition: cpp.compilerPath.contains("2010") || cpp.compilerPath.contains("VC10")
		COMPILER_NAME: "VC9"
	}

	property string COMPILER_DIR
	Properties{
		condition: cpp.debugInformation == true
		COMPILER_DIR: "Debug" + COMPILER_NAME
	}
	Properties{
		condition: cpp.debugInformation == false
		COMPILER_DIR: "Release" + COMPILER_NAME
	}

	readonly property path acfConfigurationFile: "undefined_file"
	property path trConfigurationFile: acfConfigurationFile
	property path trRegFile

	FileTagger{
		pattern: "*.arx"
		fileTags: ["arx"]
	}

	FileTagger{
		pattern: "*.xtracf"
		fileTags: ["xtracf"]
	}

	FileTagger{
		pattern: "*Pck.arp"
		fileTags: ["acf-components"]
	}

	Rule{
        id: arxcompiler
		inputs: ["arx"]
		auxiliaryInputs: ["Arxc"]

		Artifact{
			fileName: product.name + '/Generated/C' + input.baseName + '.cpp'
			fileTags: ["cpp"]
		}
		Artifact{
			fileName: product.name + '/Generated/C' + input.baseName + '.h'
			fileTags: ["hpp"]
		}

		prepare:{
			var cmd = new Command(product.buildDirectory + '/Bin/' + product.moduleProperty("cpp", "executablePrefix") + 'Arxc' + product.moduleProperty("cpp", "executableSuffix"), [
						input.fileName,
						'-config', product.moduleProperty("acf", "acfConfigurationFile"),
						'-o', outputs.cpp[0].fileName]);
			cmd.description = 'arxc ' + FileInfo.fileName(input.fileName)
			cmd.highlight = 'codegen';
			cmd.workingDirectory = product.buildDirectory + '/Bin/';

			return cmd;
		}
		explicitlyDependsOn: "Arxc"
	}

	Rule{
        id: acftransform
		inputs: ["xtracf"]

		Artifact{
			fileName: product.name + '/Generated/' + input.completeBaseName
		}

		prepare:{
			var cmd = new Command(product.buildDirectory + '/Bin/' + product.moduleProperty("cpp", "executablePrefix") + 'Acf' + product.moduleProperty("cpp", "executableSuffix"), [
						product.moduleProperty("acf", "trRegFile").fileName,
						'-config', product.moduleProperty("acf", "trConfigurationFile").fileName,
						'-input', input.fileName,
						'-o', output.fileName]);
			cmd.description = 'acf transformation ' + FileInfo.fileName(input.fileName)
			cmd.highlight = 'codegen';
			cmd.workingDirectory = 'Bin';

			return cmd;
		}
	}
}
