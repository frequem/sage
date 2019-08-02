#version 100
precision highp float;
        
uniform sampler2D tex;
uniform sampler2D depthTex;
uniform bool peel;
varying vec2 vTexCoord;
varying vec3 posNorm;

void main(){
	if(peel && posNorm.z<=texture2D(depthTex, posNorm.xy).x+0.000009){//adjust for precision
		discard;
	}
	
	gl_FragColor = texture2D(tex, vTexCoord);
}
