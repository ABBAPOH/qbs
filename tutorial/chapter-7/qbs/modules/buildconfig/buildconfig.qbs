import qbs.FileInfo

//! [1]
//! [0]
// qbs/modules/buildconfig/buildconfig.qbs
BuildConfig {
    installImportLibs: true
    property bool installPublicHeaders: false
    //! [0]

    Depends { name: "cpp" }
    Depends { name: "installpaths" }

    property bool enableRPath: true
    property stringList libRPaths: {
        if (enableRPath && cpp.rpathOrigin && product.installDir) {
            return [
                FileInfo.joinPaths(
                    cpp.rpathOrigin,
                    FileInfo.relativePath(
                        FileInfo.joinPaths('/', product.installDir),
                        FileInfo.joinPaths('/', dynamicLibrariesInstallDir)))];
        }
        return [];
    }

    cpp.rpaths: libRPaths
}
//! [1]
