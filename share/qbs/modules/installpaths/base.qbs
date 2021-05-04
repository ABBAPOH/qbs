Module {
    priority: -100

    property bool autotetect: false

    property string bin: "bin"
    property string configs: "etc"
    property string data: "share"
    property string include: "include"
    property string lib: "lib"

    // TODO: is this root project?
    property string dirSuffix: project.name.toLowerCase()
    property string libexec: "libexec/" + dirSuffix

    property string plugins: lib + "/" + dirSuffix + "/plugins"

    property string share: "share/" + dirSuffix
}
