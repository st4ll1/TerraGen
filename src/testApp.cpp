#include "testApp.h"

testApp::~testApp(){
    freeMap();
    shader.unload();
}

void testApp::freeMap(){
    for(int i = 0; i< size; i++){
        delete[] map[i];
    }
    delete map;
}

void testApp::resetMap(float value){
    for(int y = 0; y < size; y++){
        for(int x = 0; x < size; x++){
            map[x][y] = value;
        }
    }
}

void testApp::setup() {
    ofSetVerticalSync(true);
    ofDisableSetupScreen();
    width = ofGetWidth();
    height = ofGetHeight();

    shader.loadShader("shader");
    waterShader.loadShader("watershader");
    fboTexture.allocate(128,128);
    bool result;
    result = landImage.loadImage("landTexture.png");
    if (!result) {
        ofLog(OF_LOG_ERROR, "Failed to load image");
    }
    landTexture.load(&landImage, GL_CLAMP, GL_CLAMP);
    factor = 7;
    newMapsize = false;
    size = pow(2.0,factor)+1;
    drawMode = SOLID;
    waterLevel = 0.0;
    initMap();
    resetMap(0.0);
    rangeDivisor = 0.5;
    calculateMapWithSquareDiamond(0,0,size-1,RANGE,0);
    displayList = new DisplayList();  
}

void testApp::generateNoiseWaterHeightMap(){
    noiseImage.allocate(128,128,OF_IMAGE_GRAYSCALE);
    char* image = new char[128*128];   
    float elapsedTime = ofGetElapsedTimef();
    for(int y = 0; y < noiseImage.width; y++){
        for(int x = 0; x < noiseImage.height; x++){            
            image[x + noiseImage.width * y] = ofNoise(x/4.0,y/4.0,elapsedTime)*100;
        }
    }
    noiseImage.setFromPixels((unsigned char*)image,128,128,OF_IMAGE_GRAYSCALE);
    noiseTexture.load(&noiseImage,GL_CLAMP,GL_CLAMP);   
}

void testApp::generateNewMap(){
    if(newMapsize){
        freeMap();
        size = pow(2.0,factor)+1;
        initMap();
        newMapsize = false;
    }
    resetMap(0.0);
    ofSeedRandom(ofGetElapsedTimeMillis());
    calculateMapWithSquareDiamond(0,0,size-1,size/3,0);
    delete displayList;
    displayList = new DisplayList();

    ScopedDisplayListCompiler compiler(displayList);
    drawScene();   
}

void testApp::update() {
    currentTime = ofGetElapsedTimef();
    camera = Vector(size*sin(currentTime/10),size*cos(currentTime/10),50.0f);
    frustumCamera = Vector(0.0,0.0,-50);
    look = Vector();
}

void testApp::drawFrustum(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-size/2+camera.x, size/2+camera.x,
              -size/2-camera.y, size/2-camera.y,
              -frustumCamera.z, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(      
        camera.x, camera.y, frustumCamera.z,
        camera.x, camera.y, waterLevel,
        0.0,1.0,0.0
    );

    glPushMatrix();
        glTranslatef(-size/2.0,-size/2.0,0);
        displayList->call();
    glPopMatrix();
}

void testApp::drawFrustumAbove(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-size/2, size/2,
              -size/2, size/2,
              -frustumCamera.z, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(        
        frustumCamera.x, frustumCamera.y, frustumCamera.z,
        frustumCamera.x, frustumCamera.y, waterLevel,
        0.0,1.0,0.0
    );

    glPushMatrix();
        glTranslatef(-size/2.0,-size/2.0,0);
        displayList->call();
    glPopMatrix();
}

void testApp::setProjectionAndLookAt(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)width / height, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        camera.x, camera.y, camera.z,
        0.0, 0.0, 0.0,
        0.0, 0.0, 1.0);
}

void testApp::drawRenderScene(){
    glViewport(0,0,width,height);
    setProjectionAndLookAt();
    glEnable(GL_DEPTH_TEST);
    glPushMatrix();
        glTranslatef(-size/2,-size/2,0);
        displayList->call();        
    glPopMatrix();
    glPushMatrix();    
        glScalef(-1.0,1.0,1.0);                           
        generateNoiseWaterHeightMap();                
        waterShader.bind();
        //bind mirrorTexture
        glActiveTexture(GL_TEXTURE0);
        fboTexture.bind();
        waterShader.setUniformVariable1i("mirrorTexture", 0);
        //bind noiseTexture
        glActiveTexture(GL_TEXTURE1);
        noiseTexture.bind();
        waterShader.setUniformVariable1i("noiseTexture", 1);
        drawWater();
        glActiveTexture(GL_TEXTURE1);
        noiseTexture.unbind();
        glActiveTexture(GL_TEXTURE0);
        fboTexture.unbind();
        waterShader.unbind();
    glPopMatrix();  
}

void testApp::draw() {
    fboTexture.bindRenderTarget();
    glViewport(0.0, 0.0, fboTexture.getWidth(), fboTexture.getHeight());
    // clear viewport
    glClearColor(0.0, 0.0, 0.5, 1.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);      
    //frustum    
    drawFrustum();
    fboTexture.unbindRenderTarget();
    drawRenderScene(); 
    /*
    //frustum
    glViewport(width/2,0.0,width/2,height/2);
    drawFrustum();    
    //noiseimage for the water
    glViewport(0.0,0.0,width/2,height/2);
    setProjectionAndLookAt();    
    glPushMatrix();
        generateNoiseWaterHeightMap();        
        noiseTexture.bind();
        drawWater();
        noiseTexture.unbind();
    glPopMatrix();
    //watertexture
    glViewport(width/2,height/2,width/2,height/2);
    setProjectionAndLookAt();    
    glPushMatrix();
        glScalef(-1.0,1.0,1.0);                           
        generateNoiseWaterHeightMap();        
        fboTexture.bind();
        drawWater();
        fboTexture.unbind();
    glPopMatrix();   */
    
    /*
    frame capturing for video
    frameCounter++;
    ofImage screenShot;
    screenShot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());    
    screenShot.saveImage("screenShot" + ofToString(frameCounter) + ".png");*/

}

void testApp::initMap(){
    map = new float*[size];
    for(int y = 0; y < size; y++){
        map[y] = new float[size];
    }
}

void testApp::calculateMapWithSquareDiamond(int posX, int posY, int width, float range,int mode){
    if(width <=1) return;
    if(mode == 0 || mode == 2)
        map[posX+width/2][posY] = (map[posX][posY]+map[posX+width][posY])/2 + ofRandom(-range,range);
    map[posX+width/2][posY+width] = (map[posX][posY+width]+map[posX+width][posY+width])/2 + ofRandom(-range,range);
    map[posX+width][posY+width/2] = (map[posX+width][posY]+map[posX+width][posY+width])/2 + ofRandom(-range,range);
    if(mode==0 || mode == 1)
        map[posX][posY+width/2] = (map[posX][posY]+map[posX][posY+width])/2 + ofRandom(-range,range);
    map[posX+width/2][posY+width/2] = (map[posX][posY+width/2]+map[posX+width][posY+width/2]+
        map[posX+width/2][posY+width]+map[posX+width/2][posY])/4;
    calculateMapWithSquareDiamond(posX,posY, width/2,range*rangeDivisor,mode);
    calculateMapWithSquareDiamond(posX,posY+width/2,width/2,range*rangeDivisor,(mode == 0 || mode == 1? 1 : 3));
    calculateMapWithSquareDiamond(posX+width/2,posY,width/2,range*rangeDivisor,(mode == 0 || mode == 2? 2 : 3));
    calculateMapWithSquareDiamond(posX+width/2,posY+width/2,width/2,range*rangeDivisor,3);
}

void testApp::drawMap(){
    for(int y=0;y<size-1;y++){
        for(int x =0; x<size-1;x++){
            switch(drawMode){
                case SOLID:
                    glBegin(GL_POLYGON);
                    break;
                case WIRED:
                    glBegin(GL_LINE_LOOP);
                    break;
            }                
                glTexCoord2f(0.0, 0.0);
                glVertex3f(x,y,map[x][y]);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(x+1,y,map[x+1][y]);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(x+1,y+1,map[x+1][y+1]);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(x,y+1,map[x][y+1]);
            glEnd();
        }
    }
}

void testApp::drawWater(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for(int y=0;y<size-1;y++){
        for(int x =0; x<size-1;x++){            
            glBegin(GL_POLYGON);           
            glTexCoord2f((float)x/size, (float)y/size);
            glVertex3f(x-size/2.0f,y-size/2.0f,0.0);
            glTexCoord2f((float)(x+1)/size, (float)y/size);
            glVertex3f(x+1-size/2.0f,y-size/2.0f,0.0);
            glTexCoord2f((float)(x+1)/size, (float)(y+1)/size);
            glVertex3f(x+1-size/2.0f,y+1-size/2.0f,0.0);
            glTexCoord2f((float)x/size, (float)(y+1)/size);
            glVertex3f(x-size/2.0f,y+1-size/2.0f,0.0);
            glEnd();
        }
    }
    glDisable(GL_BLEND);
}

void testApp::keyPressed(int key) {
    ScreenShotApplication::keyPressed(key);
    switch(key){
        case 'n':
            generateNewMap();
            break;
        case 's':
            drawMode = SOLID;
            break;
        case 'w':
            drawMode = WIRED;
            break;
        case '-':
            if(factor > 1) factor--;
            newMapsize = true;
            break;
        case '+':
            factor++;
            newMapsize = true;
            break;
        case 'q':
            waterLevel += 10;
            break;
        case 'a':
            waterLevel -= 1;
            break;
    }
}

void testApp::keyReleased(int key) {
}

void testApp::mouseMoved(int x, int y) {
}

void testApp::mouseDragged(int x, int y, int button) {
}

void testApp::mousePressed(int x, int y, int button) {
}

void testApp::mouseReleased(int x, int y, int button) {
}

void testApp::windowResized(int w, int h) {
    width = ofGetWidth();
    height = ofGetHeight();
}

void testApp::drawScene() {
    shader.bind();
    glActiveTexture(GL_TEXTURE0);
    landTexture.bind();
    shader.setUniformVariable1i("landTexture", 0);
    glPushMatrix();
        drawMap();
    glPopMatrix();

    glActiveTexture(GL_TEXTURE0);
    landTexture.unbind();
    shader.unbind();
}
