#version 100
precision highp float;

uniform sampler2D depthTex;
uniform bool peel;
varying vec3 posNorm;
uniform vec4 color;

void main(){
	if(peel && posNorm.z<=texture2D(depthTex, posNorm.xy).x+0.000009){//adjust for precision
		discard;
	}
	
	gl_FragColor = color;
}
