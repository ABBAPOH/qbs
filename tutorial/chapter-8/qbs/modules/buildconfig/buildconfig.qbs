import qbs.FileInfo

BuildConfig {
    Depends { name: "cpp" }
    Depends { name: "installpaths" }

    installImportLibs: true
    property bool installPublicHeaders: false
    property bool staticBuild: false
    property bool enableRPath: true

    property stringList libRPaths: {
        if (enableRPath && cpp.rpathOrigin && product.installDir) {
            return [
                FileInfo.joinPaths(
                    cpp.rpathOrigin,
                    FileInfo.relativePath(
                        FileInfo.joinPaths('/', product.installDir),
                        FileInfo.joinPaths('/', dynamicLibrariesInstallDir)))
            ];
        }
        return [];
    }
}
