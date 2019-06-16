#version 100
precision mediump float;

attribute float idx;
varying vec2 vTexCoord;

void main(){
	//0->(0,0), 1->(1,0), 2->(0,1), 3->(1,1)
	vTexCoord = vec2(mod(idx, 2.0), step(2.0, idx));
	//0->(-1,-1), 1->(1,-1), 2->(-1,1), 3->(1,1)
	gl_Position = vec4(vTexCoord*2.0-1.0, 0, 1);
	
}
