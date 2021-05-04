Module {
    Depends { name: "installpaths" }
    Depends { name: "items.common" }

    readonly property bool isDynamicLibrary: product.type.contains("dynamiclibrary")
    readonly property bool isStaticLibrary: product.type.contains("staticlibrary")

    property bool install: items.common.install && !isStaticLibrary
    property string installDir: {
        if (isDynamicLibrary && qbs.targetOS.contains("windows"))
            return installpaths.bin;
        return installpaths.lib;
    }

    property bool installDebugInformation: items.common.installDebugInformation
    property string debugInformationInstallDir: installDir

    property bool installImportLib: false
    // todo: dedicated property for this?
    property string importLibInstallDir: installpaths.lib
}
