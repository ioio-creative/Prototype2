#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(0);
    
    //setupCamera
    vector<ofVideoDevice> devices = cameras[0].listDevices();
    //ofLog() << devices.size();
    for(int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
            cameras[i].setDeviceID(i);
            cameras[i].initGrabber(camW,camH);
            
        }else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    
    
    //setup button to camera ID
    //int camID = -1;
    for(int i = 0; i < totalBtn; i++){
    //    if(i % btnPerCam == 0){
    //      camID ++;
    // }
        //btnToCam[i] = camID;
        cout << "Button "<< i+1 << "is going to Camera No." <<btnToCam[i] << endl ;
        
    }
    
    
    ofSetVerticalSync(true);
    rawPhoto.allocate(1920,1080,OF_IMAGE_COLOR);
    
    //OSC SEND
    
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, SEND_PORT);
    ofSetFrameRate(60);
    
    //OSC receive
    cout << "listening for osc messages on port " << RECEIVE_PORT << "\n";
    receiver.setup(RECEIVE_PORT);
    
    //setup Serial
    serial.listDevices();
    vector<ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    serial.setup( 0 , baud );

    
    // Skeleton Tracking part
    connections = {
        {"Nose", "Left_Eye"},
        {"Left_Eye", "Left_Ear"},
        {"Nose", "Right_Eye"},
        {"Right_Eye", "Right_Ear"},
        {"Nose", "Neck"},
        {"Neck", "Right_Shoulder"},
        {"Neck", "Left_Shoulder"},
        {"Right_Shoulder", "Right_Elbow"},
        {"Right_Elbow", "Right_Wrist"},
        {"Left_Shoulder", "Left_Elbow"},
        {"Left_Elbow", "Left_Wrist"},
        {"Neck", "Right_Hip"},
        {"Right_Hip", "Right_Knee"},
        {"Right_Knee", "Right_Ankle"},
        {"Neck", "Left_Hip"},
        {"Left_Hip", "Left_Knee"},
        {"Left_Knee", "Left_Ankle"}
    };

}

//--------------------------------------------------------------
void ofApp::update(){
    getHumanFromOSC();
    
    int myByte = 0;
    if(serial.available()){
        myByte = serial.readByte();
//        if ( myByte == OF_SERIAL_NO_DATA )
//            printf("no data was read");
//        else if ( myByte == OF_SERIAL_ERROR )
//            printf("an error occurred");
//        else
            printf("myByte is %d \n", myByte);
      
        if(myByte <= 36) {
            int TempBtnID = (myByte+1)/2 ;
           // int TempBtnID = (myByte);
            printf("TempBtn is %d \n", TempBtnID);
          //  flrBtnPressed(0);
          //  flrBtnPressed(myByte);
            flrBtnPressed(TempBtnID);
            //supposedly you should call flrBtnPressed(btnID)
            
            //int TempBtnID =
            //flnBtnPressed(
        }
    }
}
//--------------------------------------------------------------
void ofApp::draw(){
    
//    if(cropPhoto.isAllocated()){
//        cropPhoto.draw(10,10);
//    }
    if(optPhoto.isAllocated()){
        optPhoto.draw(10 ,10,500,880);
        
    }
    drawParts();
    drawConnectionsOF();
    
}

// TriggerEvent
// Once Triggered,
// grab photo from camera[cameraID]
// CropA
void ofApp::flrBtnPressed(int btnID){
    
    //grab image from the designated camera according to button
    int camID = btnToCam[btnID];
    cameras[camID].update();
    ofLog() << camID;
    cout << "Current pressed Btn: "<< btnID << "- is going to Camera No." <<camID << endl ;
    rawPhoto.setFromPixels(cameras[camID].getPixels());
    
    //first crop
    int cropStartX = btnToStartPoint[btnID];
    int cropStartY = 0;
    //rawPhoto.crop(cropStartX,cropStartY,cropW,cropH);
    //cropPhoto.cropFrom(rawPhoto,cropStartX,cropStartY,cropW,cropH);
    cropPhoto.cropFrom(rawPhoto,0,0,cropW,cropH);
    
    //cropPhoto
    //suppose at the end of here I plug the cropPhoto into the openPos function
    //openPose(cropPhoto);
    //1. save to a particular location
    string path = "/Users/chunhoiwong_ioio/Documents/OpenFramework X/of_v0.10.0_osx_release/apps/myApps/Prototype2/prototype-2/bin/data/nodeGetImage/public/";
    
    if(flag<18){
        cropPhoto.save(path+ofToString(flag)+".jpg");
        
        ofxOscMessage m;
        m.setAddress("/test");
        m.addIntArg(flag);
        sender.sendMessage(m, true);
        flag++;
        if(flag == 18){
                flag = 0;
            }
    }
    
       //web server got OSC m, read photo from location
    
    //suppose it will pass the photo to runway
    //web server getSkeleton data JSON
    //web server send OSC of skeletonJSON
    
    
//OSC receive. Unpack JSON
//look for left border and right border
}
void ofApp::getHumanFromOSC(){
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        ofLog() << "OSC got through" ;
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        //probably  insert the EXPORT JSON part here
        //============================================================
        
        
        
        //============================================================

       
        // check message from nodeGetImage
        if(m.getAddress() == "/ske"){
            // both the arguments are int32's
            float center = 0;
            float leftX = 0;
            float rightX = 0;
            for(int i=0; i< m.getNumArgs();i+=3){
                
                //the received array format as ["bodyPartA","bodyPartA-X-Coor","bodyPartA-Y-Coor","bodyPartB",..............]
                // and bodyPart coordinate is format as 0.0 - 1.0, 0.5 as the middle of the photo
                if(m.getArgAsString(i) == "Left_Hip"){
                    ofLog()<< m.getArgAsString(i);
                    ofLog()<< m.getArgAsFloat(i+1);
                    ofLog()<< m.getArgAsFloat(i+2);
                    leftX = m.getArgAsFloat(i+1);
                }else if (m.getArgAsString(i) == "Right_Hip"){
                    ofLog()<< m.getArgAsString(i);
                    ofLog()<< m.getArgAsFloat(i+1);
                    ofLog()<< m.getArgAsFloat(i+2);
                    rightX = m.getArgAsFloat(i+1);
                }
                
                //print every argument's x coordinate
                ofLog()<< m.getArgAsString(i) <<" 's X cordinate in Float" ;
                ofLog()<< m.getArgAsFloat(i+1);
                

    
            }
            if(leftX != 0 && rightX !=0){
                center = (leftX + rightX)/2;
            }else if(leftX == 0){
                center = rightX;
            }else if(rightX == 0){
                center = leftX;
            }
            //translating 0.0 - 1.0 to pixel coordinate, 250 is just an arbituary number for the final photo size
            leftBorder = center * cropW - 250;
            rightBorder = center * cropW + 250;
            //ofLog() << "min : " << minX << "  max : "<< maxX;
            ofLog() << "min : " << leftBorder << "  max : "<< rightBorder;
            //crop optimal photo for display
            if(center != 0){
                optPhoto.cropFrom(cropPhoto,leftBorder,0,rightBorder - leftBorder, cropPhoto.getHeight());
                optPhoto.save(ofToString(ofGetElapsedTimef())+".jpg");
            }else{
                ofLog() << "Can't Find Center";
            }
        }else{
            // unrecognized message: display on the bottom of the screen
            string msg_string;
            msg_string = m.getAddress();
            msg_string += ": ";
            for(int i = 0; i < m.getNumArgs(); i++){
                // get the argument type
                msg_string += m.getArgTypeName(i);
                msg_string += ":";
                // display the argument - make sure we get the right type
                if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                    msg_string += ofToString(m.getArgAsInt32(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                    msg_string += ofToString(m.getArgAsFloat(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                    msg_string += m.getArgAsString(i);
                }
                else{
                    msg_string += "unknown";
                }
            }
           
        }
     if(m.getAddress() == "/sket"){
         // get the next OSC message
         //ofxOscMessage m;
         //receiver.getNextMessage(m);
         
         //        grab the data
         string data = m.getArgAsString(0);
         
         //        parse it to JSON
         results.parse(data);
         
         //        grab the first human detected in the Array
         humans = results[0];
         ofLog() << "humans JSON size: " <<humans.size() ;
         ofLog() << "Has OSC Msg for one Human : "<< humans;
         results.save("result_output_pretty.json", true);
         humans.save("SINGLEhumans_output_pretty.json",true);
         
        
         
     }
        
        
}
    
    
    
    
    
}



// A function to draw humans body parts as circles

void ofApp::drawParts(){
    for(int h = 0; h < results.size(); h++) {
        ofxJSONElement human = results[h];
        // Now that we have one human, let's draw its body parts
        for (int b = 0; b < human.size(); b++) {
            ofxJSONElement body_part = human[b];
            // Body parts are relative to width and weight of the input
            float x = body_part[1].asFloat();
            float y = body_part[2].asFloat();
            ofDrawEllipse(x * camW, y * camH, 10, 10);
        }
    }
}

//------------------------------------------------- updated at 2018-08-29
void ofApp::drawConnectionsOF(){
    for(int h = 0; h < results.size(); h++) {
        ofxJSONElement human = results[h];
        // Now that we have a human, let's draw its body
        // connections start by looping through all body
        // connections and matching only the ones we need.
        for(int c = 0; c < connections.size(); c++){
            ofxJSONElement start;
            ofxJSONElement end;
            // Check if we have a pair in the current body parts
            for(int b = 0; b < human.size(); b++) {
                ofxJSONElement body_part = human[b];
                string name = body_part[0].asString();
                if (name == connections[c][0]){
                    start = body_part;
                } else if (name == connections[c][1]){
                    end = body_part;
                }
            }
            if (start.size() > 0 && end.size() > 0){
                float x1 = start[1].asFloat() * camW;
                float y1 = start[2].asFloat() * camH;
                float x2 = end[1].asFloat() * camW;
                float y2 = end[2].asFloat() * camH;
                ofDrawLine(x1, y1, x2, y2);
            }
        }
    }
}
//--------------------------------------------------------------



void ofApp::keyPressed(int key){
    if(key == '0'){
        flrBtnPressed(0);
    }else if(key == '1'){
        flrBtnPressed(1);
    }else if(key == '2'){
        flrBtnPressed(2);
    }else if(key == '3'){
        flrBtnPressed(3);
    }else if(key == '4'){
        flrBtnPressed(4);
    }else if(key == '5'){
        flrBtnPressed(5);
    }else if(key == '6'){
        flrBtnPressed(6);
    }
    
}
