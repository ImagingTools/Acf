function getGeneratedPath()
{
    var outputDir = product.moduleProperty("acf", "generatedOutputDir");
    if (outputDir == null){
        return "GeneratedFiles/" + product.name;
    }
    else{
        return outputDir;
    }
}
