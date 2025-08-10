import qbs.FileInfo
Module {

    Depends { name: "cpp" }
    Depends { name: "installpaths" }
    Depends { name: "config.install" }

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
                        FileInfo.joinPaths('/', config.install.dynamicLibrariesInstallDir)))];
        }
        return [];
    }

    cpp.rpaths: libRPaths
}

