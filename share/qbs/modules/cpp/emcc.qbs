import qbs.FileInfo
import qbs.ModUtils

GenericGCC
{
    condition: qbs.toolchain && qbs.toolchain.includes("emscripten")
    priority: -50
    cCompilerName: "emcc"
    cxxCompilerName: "em++"
    archiverName: "emar"
    dynamicLibrarySuffix: ".so"
    dynamicLibraryPrefix: "lib"
    executableSuffix: ".js"
    rpathOrigin: "irrelevant"//to pass tests
    imageFormat: "wasm"

    property string emsdkDir
    property string emsdkNode
    property string emsdkPython
    property string javaHome

    probeEnv: ({
        "EMSDK": emsdkDir,
        "EMSDK_NODE": emsdkNode,
        "EMSDK_PYTHON": emsdkPython,
        "JAVA_HOME": javaHome
    })

    toolchainInstallPath: FileInfo.cleanPath(emsdkDir + "/upstream/emscripten/")

    setupBuildEnvironment: {
        for (var key in product.cpp.probeEnv) {
            var v = new ModUtils.EnvironmentVariable(key);
            v.value = product.cpp.probeEnv[key];
            v.set();
        }
    }

    validate: {
        if (!emsdkDir)
            throw "Emsdk root directory is not set";
    }
}
