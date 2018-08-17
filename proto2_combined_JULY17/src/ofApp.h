#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxJSON.h"

//==============================
#define totalCamera 5
#define totalBtn 18
#define camDeviceA 0
#define camDeviceB 1
#define camDeviceC 4
#define camDeviceD 2

#define camW 1920
#define camH 1080
#define btnPerCam 6

#define cropW 1920
#define cropH 880

//==============================


#define RECEIVE_PORT 3334
#define NUM_MSG_STRINGS 20


#define HOST "localhost"
#define SEND_PORT 12000


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
    
        // TriggerEvent
        // Once Triggered,
        // grab photo from camera[cameraID]
        // CropA
        void flrBtnPressed(int ID);
    
        int flag = 0;
        // cropPhoto TriggerEvent
        // Once Updated,
        // send cropPhoto to openPose
        // openPose crop
        // return optPhoto
        
        // optPhoto TriggerEvent
        // Once Updated,
        // Display
        
        //cameras array
        ofVideoGrabber cameras[totalCamera];
        
        //buttons to Cam array [buttonID] i.e.{0,0,0,1,1,1,2,2,2}
        int btnToCam[totalBtn] = {4,4,4,4,4,0,0,0,0,1,1,1,1,1,2,2,2,2};
    //int btnToCam2[36]={camDeviceD,camDeviceA,camDeviceA,camDeviceA,camDeviceA,camDeviceA,camDeviceA,camDeviceA,camDeviceA,camDeviceB,camDeviceB,camDeviceB,camDeviceB,camDeviceB,camDeviceB,camDeviceB,camDeviceB,camDeviceC,camDeviceC,camDeviceC,camDeviceC,camDeviceC,camDeviceC,camDeviceC,camDeviceC,camDeviceD,camDeviceD,camDeviceD,camDeviceD,camDeviceD,camDeviceD,camDeviceD};
        //int btnToCam[totalBtn] ;
        
        //Dimension array [buttonID] = {startPoint0, StartPoint1,...,startPoint17}
        //btnToStartPoint[] = {0,320,640,960,1280,1600,0,320,640,960,1280,1600,0,320,640,960,1280,1600}
        int btnToStartPoint[totalBtn] = {0,1200,1000,800,600,400,200,320,640,960,1280,1600,0,320,640,960,1280,1600};
        
    
        int leftBorder, rightBorder;
        //rawPhoto, cropPhoto, optPhoto
        ofImage rawPhoto;
        ofImage cropPhoto;
        ofImage optPhoto;
    
        //ofxOscSender
    
        ofxOscSender sender;
    
    
        //OSC reveice and final crop pro8l
        void getHumanFromOSC();
        
        ofxOscReceiver receiver;
        
        // This array will hold the results parsed from the OSC message string
        ofxJSONElement results;
        // This array will hold all the humans detected
        ofxJSONElement humans;
        
        vector<vector<string>> connections;;
    
        //serial
        ofSerial serial;
        int baud = 9600;

				
};
