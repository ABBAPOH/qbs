Project {
    Product {
        type: "application"
        consoleApplication: true
        name: "i"

        property bool dummy: { console.info("object suffix: " + cpp.objectSuffix); }

        Depends {
            name: "Qt.core"
        }

        files: [
            "bla.cpp",
            "bla.h"
        ]
    }
}

