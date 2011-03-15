uniform sampler2D noiseTexture;

void main() {
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Vertex.z = gl_Vertex.z + texture2D(noiseTexture, gl_TexCoord[0]).r*5;    	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
