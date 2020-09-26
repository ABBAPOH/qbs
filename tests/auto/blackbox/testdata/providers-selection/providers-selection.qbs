Project {
    property stringList wantedProviders: "provider-a"
    name: "project"
    Project {
        name: "innerProject"
        providers: project.wantedProviders
        Product {
            name: "p1"
            Depends { name: "qbsmetatestmodule" }
            property bool dummy: {
                console.info("p1.qbsmetatestmodule.prop: " + qbsmetatestmodule.prop);
            }
        }
    }

    Product {
        providers: project.wantedProviders
        name: "p2"
        Depends { name: "qbsmetatestmodule" }
        property bool dummy: {
            console.info("p2.qbsmetatestmodule.prop: " + qbsmetatestmodule.prop);
        }
    }

    Product {
        name: "p3"
        Depends { name: "qbsmetatestmodule"; providers: project.wantedProviders }
        property bool dummy: {
            console.info("p3.qbsmetatestmodule.prop: " + qbsmetatestmodule.prop);
        }
    }
}
