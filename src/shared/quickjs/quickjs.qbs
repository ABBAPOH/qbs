StaticLibrary {
    name: "quickjs"

    Depends { name: "qbsbuildconfig" }
    Depends { name: "cpp" }

    cpp.cLanguageVersion: qbs.toolchain.contains("msvc") ? "c11" : "gnu11"
    cpp.defines: ['CONFIG_VERSION="2021-03-27"'].concat(qbsbuildconfig.dumpJsLeaks
                                                        ? "DUMP_LEAKS" : [])
    cpp.warningLevel: "none"

    files: [
        "builtin-array-fromasync.h",
        "cutils.c",
        "cutils.h",
        "libbf.c",
        "libbf.h",
        "libregexp-opcode.h",
        "libregexp.c",
        "libregexp.h",
        "libunicode-table.h",
        "libunicode.c",
        "libunicode.h",
        "list.h",
        "quickjs-atom.h",
        "quickjs-c-atomics.h",
        "quickjs-opcode.h",
        "quickjs.c",
        "quickjs.h",
        "xsum.c",
        "xsum.h"
    ]

    Export {
        Depends { name: "cpp" }
        cpp.systemIncludePaths: [exportingProduct.sourceDirectory]
    }
}
