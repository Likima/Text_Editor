#version 330 core 

void main() {
    // Extract the x and y positions based on the vertex ID
    float x = float(gl_VertexID & 1);       // 0 for even IDs, 1 for odd IDs
    float y = float((gl_VertexID >> 1) & 1); // 0 for IDs 0,1, 1 for IDs 2,3

    // Map x and y to the range [-0.5, 0.5]
    gl_Position = vec4(x - 0.5, y - 0.5, 0.0, 1.0);
}
