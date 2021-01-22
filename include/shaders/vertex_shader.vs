attribute vec4 position;
attribute vec4 vertex_colour;

varying vec4 vertexColor;


// uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * vec4(position.xyz*1.2, 1.0);
  vertexColor = vec4(vertex_colour.xyz,1.0);
}