Module {
    Depends { name: "installpaths" }
    Depends { name: "items.common" }

    property bool install: items.common.install
    property string installDir: installpaths.plugins
    property bool installDebugInformation: items.common.installDebugInformation
    property string debugInformationInstallDir: installDir
}
