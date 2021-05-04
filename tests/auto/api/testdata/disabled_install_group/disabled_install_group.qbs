CppApplication {
    consoleApplication: true
    files: "main.cpp"
    config.build.install: false
    Group {
        condition: false
        qbs.install: true
        fileTagsFilter: product.type
    }
}
