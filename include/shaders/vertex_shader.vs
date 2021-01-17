attribute vec4 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * model * vec4(position.x*0.8,position.y*0.8,position.z*0.8, 1.0);
}