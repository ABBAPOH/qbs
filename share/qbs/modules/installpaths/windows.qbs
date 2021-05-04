import "base.qbs" as Base

Base {
    priority: 0
    condition: qbs.targetOS.contains("windows")

    libexec: bin
}
