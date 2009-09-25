var projectExt = "vcproj";
var projectExp = new RegExp(".*\." + projectExt + "$");
var solutionExt = "sln";
var solutionExt = new RegExp(".*\." + solutionExt + "$");


function TransformProject(fileSystem, shell, inputPath, outputPath, beQuiet, isTest)
{
	if (!beQuiet){
		WScript.Echo("Convert\nfrom " + inputPath + "\n to " + outputPath);
	}

    var inputFile = fileSystem.OpenTextFile(inputPath, 1);

    var text = inputFile.readAll();
    inputFile.close();
    var re1 = /VC8/g;
    text = text.replace(re1, "VC9");
    text = text.replace("Version=\"8,00\"", "Version=\"9,00\"");

    var outputFile = fileSystem.OpenTextFile(outputPath, 2, true);
    outputFile.writeLine(text);
    inputFile.close();

	return outputPath + "\n";
}


function ProcessFolder(fileSystem, shell, folder, parentFolder, beQuiet, isTest)
{
	var retVal = new String;

	if ((parentFolder != null) && (folder.Name.match("VC8") != null)){
		var fileIter = new Enumerator(folder.files);
		for (; !fileIter.atEnd(); fileIter.moveNext()){
			var file = fileIter.item();
			
			var isProject = projectExp.exec(file.Name);
			var isSolution = solutionExt.exec(file.Name);

			if (isProject || isSolution){
				var fileName = String(file.Name);
				var projectName = fileName.substr(0, fileName.length - projectExt.length - 1);
				var outputDir = parentFolder + "/VC9";

				if (!fileSystem.FolderExists(outputDir)){
					if (!beQuiet){
						WScript.Echo("Create folder " + outputDir);
					}

					fileSystem.CreateFolder(outputDir);
				}

				retVal += TransformProject(
				            fileSystem,
							shell,
							folder + "/" + file.Name,
							outputDir + "/" + file.Name,
							beQuiet,
							isTest);
			}
		}
	}

	var subFolderIter = new Enumerator(folder.SubFolders);
	for (; !subFolderIter.atEnd() && (!isTest || retVal ==""); subFolderIter.moveNext()){
		var childFile = subFolderIter.item();
		retVal += ProcessFolder(fileSystem, shell, childFile, folder, beQuiet, isTest);
	}
	
	return retVal;
}


var fileSystem = WScript.CreateObject("Scripting.FileSystemObject");
var fullPath = fileSystem.GetParentFolderName(WScript.ScriptFullName);
var shell = WScript.CreateObject("WScript.Shell");

var isTest = false;
var beQuiet = false;

for (var i = 0; i < WScript.Arguments.length; ++i){
    var argument = WScript.Arguments(i).toString();
    if (argument.substr(0, 1) == "-"){
        if (argument.toUpperCase() == "-T"){
            isTest = true;
        }
        else if (argument.toUpperCase() == "-Q"){
            beQuiet = true;
        }
        else{
			WScript.Echo("Unknown parameter: " + argument);
        }
    }
    else{
		WScript.Echo("Unknown parameter format: " + argument);
    }
}

var message = ProcessFolder(fileSystem, shell, fileSystem.GetFolder("."), null, beQuiet, isTest);
if (message.length > 1){
    WScript.Echo("Following projects was converted:\n\n" + message);
}
else{
    WScript.Echo("No project was converted");
}
   

WScript.Quit(0);

