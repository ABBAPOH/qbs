Module {
    Depends { name: "installpaths" }

    property bool install: true

    property string binariesInstallDir: installpaths.bin
    property string applicationsInstallDir: installpaths.applications

    property bool installDynamicLibraries: install
    property string dynamicLibrariesInstallDir: qbs.targetOS.contains("windows")
        ? installpaths.bin : installpaths.lib

    property bool installStaticLibraries: false
    property string staticLibrariesInstallDir: installpaths.lib

    property bool installFrameworks: install
    property string frameworksInstallDir: installpaths.library + "/" + installpaths.frameworks

    property bool installImportLibraries: false
    property string importLibrariesInstallDir: installpaths.lib

    property bool installPlugins: install
    property string pluginsInstallDir: installpaths.plugins

    property bool installLoadableModules: install
    property string loadableModulesInstallDir: installpaths.library + "/" + installpaths.frameworks

    property bool installDebugInformation: install
    property string debugInformationInstallDir
}
