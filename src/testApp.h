#ifndef TEST_APP
#define TEST_APP

#define RANGE 100
#define SOLID 0
#define WIRED 1
#define TEXTURED 2

#include "ofMain.h"
#include "ofxShader/ofxShader.h"
#include "Texture.h"
#include "FboTexture.h"
#include "Vector.h"
#include "DisplayList.h"
#include "noise.h"

class ScreenShotApplication: public ofBaseApp {
public:
    virtual void keyPressed(int key) {
        switch (tolower(key)) {
            case 'p':
                ofImage screenShot;
                screenShot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
                screenShot.saveImage("screenShot.png");
        }
    }
};

class testApp: public ScreenShotApplication {
public:
    void setup();
    void update();
    void draw();
    ~testApp();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);

private:
    void setupGui();

    void setupViewport(int width, int height);
    void setupCamera(float z);
    void initMap();
    void calculateMapWithSquareDiamond(int posX, int posY, int width, float range, float random);
    void calculateMapWithSquareDiamond(int posX, int posY, int width, float range, int mode);
    void drawMap();
    void generateNewMap();
    void drawWiredMap();
    void drawScene();
    void drawRenderScene();
    void drawWater();
    void drawQuad(int minX, int maxX, int minY, int maxY);

    void drawFrustum();
    void drawFrustumAbove();
    void drawViewWithCameras();
    void generateNoiseWaterHeightMap();
    void setProjectionAndLookAt();

    float **map;
    void resetMap(float value);
    void freeMap();
    int factor;
    int size;
    int drawMode;
    float rangeDivisor;
    bool newMapsize;
    float waterLevel;
    float a,b,c,d,e, f,g,h,i;
    
    ofxShader shader;
    ofImage landImage;
    Texture landTexture;
    ofxShader waterShader;
    ofImage noiseImage;  
    Texture noiseTexture;

    Vector camera, frustumCamera, look;
    FboTexture fboTexture;
    DisplayList* displayList;
    //window size
    float width;
    float height;
    float currentTime;
};


#endif
