CppApplication {
    consoleApplication: true
    files: "main.cpp"
    buildconfig.install: false
    Group {
        condition: false
        qbs.install: true
        fileTagsFilter: product.type
    }
}
