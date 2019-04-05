#version 100

uniform vec2 windowSize;

attribute vec2 position;

void main(){
	//orthographic projection with left=0,right=winSize.x,bottom=0,top=winSize.y,near=0,far=1
    mat4 projection = mat4( 
		vec4(2.0/windowSize.x, 0, 0, 0),
		vec4(0, 2.0/windowSize.y, 0, 0),
		vec4(0, 0, -2.0, 0),
		vec4(-1.0, -1.0, -1.0, 1)
	);
	
	gl_Position = projection*vec4(position, 0, 1);
}
