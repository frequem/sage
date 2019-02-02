#version 120

uniform mat4 projection;
uniform mat4 modelview;

attribute vec2 position;
attribute vec2 texCoord;

void main(){  
	gl_TexCoord[0].st = texCoord;
	gl_Position = projection*(modelview*vec4(position, 0, 1));
}
