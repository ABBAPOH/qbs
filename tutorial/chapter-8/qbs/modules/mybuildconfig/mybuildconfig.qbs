import qbs.FileInfo

Module {
    Depends { name: "cpp" }
    Depends { name: "installpaths" }

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
                        FileInfo.joinPaths('/', installpaths.lib)))
            ];
        }
        return [];
    }
}
