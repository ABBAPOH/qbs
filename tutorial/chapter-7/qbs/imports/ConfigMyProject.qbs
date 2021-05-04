//! [1]
import qbs.FileInfo
//! [0]
// qbs/imports/ConfigMyProject.qbs
Module {
    property bool installPublicHeaders: false
    //! [0]

    Depends { name: "cpp" }
    Depends { name: "installpaths" }
    Depends { name: "config.build" }

    property bool enableRPath: true
    property stringList libRPaths: {
        if (enableRPath && cpp.rpathOrigin && product.installDir) {
            return [
                FileInfo.joinPaths(
                    cpp.rpathOrigin,
                    FileInfo.relativePath(
                        FileInfo.joinPaths('/', product.installDir),
                        FileInfo.joinPaths('/', config.build.dynamicLibrariesInstallDir)))];
        }
        return [];
    }

    cpp.rpaths: libRPaths
}
//! [1]
