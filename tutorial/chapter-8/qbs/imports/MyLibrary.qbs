// ![0]
Library {
    version: project.version

    property pathList publicHeaders
    Depends { name: "cpp" }
    Depends { name: "buildconfig" }
    Depends { name: "installpaths" }

    type: buildconfig.staticBuild ? "staticlibrary" : "dynamiclibrary"

    Group {
        condition: publicHeaders.length > 0
        name: "Public Headers"
        prefix: product.sourceDirectory + "/"
        files: publicHeaders
        qbs.install: buildconfig.installPublicHeaders
        qbs.installDir: installpaths.include
    }

    readonly property string _nameUpper : name.replace(" ", "_").toUpperCase()
    property string libraryMacro: _nameUpper + "_LIBRARY"
    property string staticLibraryMacro: _nameUpper + "_STATIC_LIBRARY"
    cpp.defines: buildconfig.staticBuild ? [staticLibraryMacro] : [libraryMacro]
    cpp.sonamePrefix: qbs.targetOS.contains("darwin") ? "@rpath" : undefined

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [exportingProduct.sourceDirectory]
        cpp.defines: exportingProduct.buildconfig.staticBuild
            ? [exportingProduct.staticLibraryMacro] : []
    }

    Depends { name: "bundle" }
    bundle.isBundle: false
}
// ![0]
