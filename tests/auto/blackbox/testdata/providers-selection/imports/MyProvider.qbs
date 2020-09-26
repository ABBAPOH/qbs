import qbs.File
import qbs.FileInfo
import qbs.TextFile

ModuleProvider {
    property string someProp
    relativeSearchPaths: {
        console.info("Running setup script for " + name);
        var moduleDir = FileInfo.joinPaths(outputBaseDir, "modules", name.replace(".", "/"));
        File.makePath(moduleDir);
        var module = new TextFile(FileInfo.joinPaths(moduleDir, "module.qbs"), TextFile.WriteOnly);
        module.writeLine("Module {");
        module.writeLine("    Depends { name: 'cpp' }");
        module.writeLine("    property string prop: '" + someProp + "'");
        module.writeLine("}");
        module.close();

        return "";
    }

}
