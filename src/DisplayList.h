#ifndef DISPLAY_LIST_H
#define DISPLAY_LIST_H

#include "ofMain.h"

class DisplayList {

public:
    DisplayList(){
        list = glGenLists(1);
    }

    ~DisplayList(){
        glDeleteLists(list,1);
    }


    void call(){
        glCallList(list);
    }
private:
    GLuint list;
    void beginCompile(){
        glNewList(list, GL_COMPILE);
    }

    void endCompile(){
        glEndList();
    }
    friend class ScopedDisplayListCompiler;
};

class ScopedDisplayListCompiler{
public:
    ScopedDisplayListCompiler(DisplayList *displayList){
        this->displayList = displayList;
        displayList->beginCompile();
    }

    ~ScopedDisplayListCompiler(){
        displayList->endCompile();
    }

private:
    DisplayList *displayList;
};

#endif
