CppApplication {
    consoleApplication: true
    files: "main.cpp"
    items.common.install: false
    Group {
        condition: false
        qbs.install: true
        fileTagsFilter: product.type
    }
}
