uniform float time;
uniform sampler2D mirrorTexture;
uniform sampler2D noiseTexture;

void main(void) {
    gl_FragColor =
        texture2D(mirrorTexture, gl_TexCoord[0].st) +
        texture2D(noiseTexture, gl_TexCoord[0].st);
    gl_FragColor.a  = 0.4;    
}
