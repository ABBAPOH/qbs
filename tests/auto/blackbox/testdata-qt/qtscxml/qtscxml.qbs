import qbs.Environment
import qbs.FileInfo
import qbs.Utilities

Project {
    QtApplication {
        property bool skip: {
            var result = qbs.targetOS.contains("ios");
            if (result)
                console.info("Skip this test");
            return result;
        }
        name: "app"
        Depends { name: "Qt.scxml"; required: false }

        Properties {
            condition: Qt.scxml.present && Utilities.versionCompare(Qt.core.version, "5.13.0") != 0
            cpp.defines: ["HAS_QTSCXML"]
        }

        Qt.scxml.className: "QbsStateMachine"
        Qt.scxml.namespace: "QbsTest"
        Qt.scxml.generateStateMethods: true

        files: ["main.cpp"]
        Group {
            files: ["dummystatemachine.scxml"]
            fileTags: ["qt.scxml.compilable"]
        }
    }

    Product {
        name: "runner"
        type: ["runner"]
        Depends { name: "app" }
        Rule {
            inputsFromDependencies: ["application"]
            outputFileTags: ["runner"]
            prepare: {
                var cmd = new Command(input.filePath);
                cmd.description = "running " + input.filePath;
                var pathVar;
                var pathValue;
                if (product.qbs.hostOS.contains("windows")) {
                    pathVar = "PATH";
                    pathValue = FileInfo.toWindowsSeparators(input["Qt.core"].binPath);
                } else {
                    pathVar = "LD_LIBRARY_PATH";
                    pathValue = input["Qt.core"].libPath;
                }
                var oldValue = Environment.getEnv(pathVar) || "";
                var newValue = pathValue + product.qbs.pathListSeparator + oldValue;
                cmd.environment = [pathVar + '=' + newValue];
                return [cmd];
            }
        }
    }
}
