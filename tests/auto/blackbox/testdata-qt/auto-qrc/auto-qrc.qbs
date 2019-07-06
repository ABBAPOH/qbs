Project {
    QtApplication {
        property bool skip: {
            var result = qbs.targetOS.contains("ios");
            if (result)
                console.info("Skip this test");
            return result;
        }

        name: "app"
        files: ["main.cpp"]

        Group {
            prefix: "qrc-base/"

            Qt.core.resourcePrefix: "/thePrefix"
            Qt.core.resourceSourceBase: "qrc-base"

            files: ["resource1.txt"]
            fileTags: ["qt.core.resource_data"]

            Group {
                prefix: "qrc-base/subdir/"

                Qt.core.resourceSourceBase: "qrc-base/subdir"

                files: ["resource2.txt"]

                Group {
                    prefix: "qrc-base/subdir/"

                    Qt.core.resourcePrefix: "/theOtherPrefix"

                    files: ["resource3.txt"]
                }
            }
        }
    }
}
