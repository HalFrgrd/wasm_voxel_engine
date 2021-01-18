precision mediump float;

uniform vec3 main_colour;

void main()
{
  gl_FragColor = vec4(main_colour.xyz,1.0);
}