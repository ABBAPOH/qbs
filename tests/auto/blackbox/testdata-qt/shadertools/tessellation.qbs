import qbs

QtApplication {
    name: "shadertools-test"
    type: "application"

    Depends { name: "Qt.shadertools" }
    Qt.shadertools.glslVersions: ["410", "320es"]

    Group {
        name: "tessellation shaders"
        files: ["tessellation_shader.tesc"]
        Qt.shadertools.tessellation: true
        Qt.shadertools.tessellationVertexCount: 3
        Qt.shadertools.tessellationMode: "triangles"
    }

    files: ["tessellation_shader.vert", "main.cpp"]
}