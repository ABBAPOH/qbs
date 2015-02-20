import qbs

Module {
    property string propDependency: "value in lowerlevel module"
    property string prop: propDependency

    Rule {
        inputs: ["dummy-input"]
        Artifact {
            filePath: "dummy.out"
            fileTags: "mytype"
        }
        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.sourceCode = function() { };
            var prop = product.moduleProperty("lowerlevel", "prop");
            cmd.description = "lowerlevel.prop is '" + prop + "'.";
            return [cmd];
        }
    }
}
