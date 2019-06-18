#version 100
precision highp float;

uniform vec2 windowSize;
attribute vec3 position;
attribute vec2 texCoord;
varying vec2 vTexCoord;

void main(){	
	vTexCoord = texCoord;
	
	//orthographic projection with left=0,right=winSize.x,bottom=0,top=winSize.y,near=-10000,far=10000 (in column order!)
    mat4 projection = mat4( 
		vec4(2.0/windowSize.x, 0, 0, 0),
		vec4(0, 2.0/windowSize.y, 0, 0),
		vec4(0, 0, -0.0001, 0),
		vec4(-1, -1, 0, 1)
	);
	
	gl_Position = projection*vec4(position, 1);
}
