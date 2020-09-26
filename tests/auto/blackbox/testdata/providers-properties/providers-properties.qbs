Product {
    providers: ["provider_a", "provider_b"]
    name: "p"
    Depends { name: "qbsmetatestmodule" }
    Depends { name: "qbsfallbackmodule" }
    moduleProviders.qbsmetatestmodule.provider_a.someProp: "someValue"
    moduleProviders.qbsfallbackmodule.provider_b.someProp: "otherValue"
    property bool dummy: {
        console.info("p.qbsmetatestmodule.prop: " + qbsmetatestmodule.prop);
        console.info("p.qbsfallbackmodule.prop: " + qbsfallbackmodule.prop);
    }
}
