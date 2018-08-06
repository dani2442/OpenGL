#version 330

layout (location = 0) in vec3 pos;

out vec4 vCol;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out gl_PerVertex
{
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
}vert;

void main()
{
	gl_Position = projection * view * model * vec4(pos.x,pos.y,pos.z, 1.0);
	vCol = vec4(0.5,0.0,0.0, 1.0f);
}