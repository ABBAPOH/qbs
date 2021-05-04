import "base.qbs" as Base

Base {
    priority: 0
    condition: qbs.targetOS.contains("darwin")

    Depends { name: "bundle" }

    bin: bundle.isBundle ? "Applications" : base
    lib: bundle.isBundle ? "Library/Frameworks" : base
    libexec: bundle.isBundle ? "Resources/libexec" : base
    share: bundle.isBundle ? "Resources" : base
    plugins: bundle.isBundle ? "PlugIns" : base
    // should we append product name here?
    configs: "Library"
}
