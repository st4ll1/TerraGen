void main() {
	gl_TexCoord[0].s = (gl_Vertex.z + 10 ) * 0.03;
    gl_TexCoord[0].s += (sin(0.47*gl_Vertex.x)+cos(0.43*gl_Vertex.x)+sin(0.37*gl_Vertex.y)+cos(0.53*gl_Vertex.y)) * gl_Vertex.z/500.0;
	gl_TexCoord[0].t = 1.0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
