attribute vec4 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * model * vec4(position.xyz*1.2, 1.0);
}