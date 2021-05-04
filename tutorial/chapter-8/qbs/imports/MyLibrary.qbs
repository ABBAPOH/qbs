// ![0]
Library {
    version: project.version

    property pathList publicHeaders
    Depends { name: "cpp" }
    Depends { name: "mybuildconfig" }
    Depends { name: "installpaths" }

    type: mybuildconfig.staticBuild ? "staticlibrary" : "dynamiclibrary"

    Group {
        condition: publicHeaders.length > 0
        name: "Public Headers"
        prefix: product.sourceDirectory + "/"
        files: publicHeaders
        qbs.install: mybuildconfig.installPublicHeaders
        qbs.installDir: installpaths.include
    }

    readonly property string _nameUpper : name.replace(" ", "_").toUpperCase()
    property string libraryMacro: _nameUpper + "_LIBRARY"
    property string staticLibraryMacro: _nameUpper + "_STATIC_LIBRARY"
    cpp.defines: mybuildconfig.staticBuild ? [staticLibraryMacro] : [libraryMacro]
    cpp.sonamePrefix: qbs.targetOS.contains("darwin") ? "@rpath" : undefined

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [exportingProduct.sourceDirectory]
        cpp.defines: exportingProduct.mybuildconfig.staticBuild
            ? [exportingProduct.staticLibraryMacro] : []
    }

    Depends { name: "bundle" }
    bundle.isBundle: false
}
// ![0]
