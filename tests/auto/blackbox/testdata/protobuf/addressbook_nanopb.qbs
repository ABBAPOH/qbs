import qbs

CppApplication {
    condition: {
        var result = qbs.targetPlatform === qbs.hostPlatform;
        if (!result)
            console.info("targetPlatform differs from hostPlatform");
        return result && hasProtobuf;
    }
    name: "addressbook_nanopb"
    consoleApplication: true

    Depends { name: "cpp" }
    cpp.cxxLanguageVersion: "c++11"
    cpp.minimumMacosVersion: "10.8"

    Depends { name: "protobuf.nanopb"; required: false }
    property bool hasProtobuf: {
        console.info("has protobuf: " + protobuf.nanopb.present);
        return protobuf.nanopb.present;
    }

    files: [
        "addressbook_nanopb.proto",
        "main_nanopb.cpp",
    ]
}
