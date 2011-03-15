uniform sampler2D landTexture;

void main(void) {
    gl_FragColor =
        texture2D(landTexture, gl_TexCoord[0].st);
    gl_FragColor.a  = 1.0;
}
