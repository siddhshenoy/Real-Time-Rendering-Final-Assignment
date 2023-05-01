#version 330 core

layout (location = 0) in vec4 Vertex;

out vec2 TexCoords;
out vec2 VertexPos;

uniform mat4 frame_transform;
void main() {
    gl_Position = frame_transform * vec4(Vertex.x, Vertex.y, 0.0, 1.0);
    TexCoords = Vertex.zw;
}