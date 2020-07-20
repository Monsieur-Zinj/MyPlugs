#include "plugin.hpp"
//#include "DynamicComponents.hpp"

template <typename BASE>
struct MLMediumLight : BASE {
	MLMediumLight() {
		this->box.size = Vec(22.0, 22.0);
	}
};

template <typename BASE>
struct Small : BASE {
	Small() {
		this->box.size = Vec(10.0, 10.0);
	}
};

struct Cell{

bool active=true;
bool state=false;
bool fixstate=false;
int voisin=0;
bool prevstate=false;
int group=0;


Cell(){}


};


struct CovGate: Module {

	enum ParamIds {
        CONT_PARAM,
        CURE_PARAM,
        FIX_PARAM,
        GROUP_PARAM,
        ENUMS(CELL_PARAM,16),

		NUM_PARAMS
        
	};
	enum InputIds {
        ENUMS(CLOCK_INPUT,1),
        GATE_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
        GATE_OUTPUT,
        ENUMS(ORGROUP_OUTPUT,4),
        ENUMS(ANDGROUP_OUTPUT,4),
        
		NUM_OUTPUTS
	};
	enum LightIds {
        ENUMS(CELL_LIGHT,32),
        FIX_LIGHT,
        GROUP_LIGHT,
		NUM_LIGHTS
	};

    Cell cell[16];

    // Clock stuff 
    int time=0;
    int steptime=0;
    int step=0;
    int inctime = 0;
    int inc=0;
    int stepcount=0;


    
    rack::dsp::SchmittTrigger  ClockTrig;
    float lastparam[16]={0.f};
    float lastfixparam=0.0f;
    float lastgroupparam=0.0f;
    bool fix=false;
    bool group=false;



    bool isgateraise(float gate,float lastgate){
        if (lastgate<gate){return true;}
        else {return false;}        
    }
    
    bool isgatefall(float gate,float lastgate){
        if (lastgate>gate){return true;}
        else {return false;}        
    }

    void setGreen(int i){
        lights[CELL_LIGHT+2*i].setBrightness(1.f);
        lights[CELL_LIGHT+2*i+1].setBrightness(0.f);
    }
    
    void setRed(int i){
        lights[CELL_LIGHT+2*i].setBrightness(0.f);
        lights[CELL_LIGHT+2*i+1].setBrightness(1.f);
    }

    void setGray(int i){
        lights[CELL_LIGHT+2*i].setBrightness(0.f);
        lights[CELL_LIGHT+2*i+1].setBrightness(0.f);
    }
    void setOrange(int i){
        lights[CELL_LIGHT+2*i].setBrightness(1.f);
        lights[CELL_LIGHT+2*i+1].setBrightness(1.f);
    }

    
    


	CovGate() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	
		configParam(CONT_PARAM, 0.0f, 100.0f, 10.0f, "CONT");	
        configParam(CURE_PARAM, 0.0f, 100.0f, 10.0f, "CURE");	
		//configParam(DIV_PARAM, -5.f, 5.f, 0.f, "");
        }
    
    //void onAdd() override {for (int i=0;i<8;i++){outputs[POLYOUT_OUTPUT+i].setChannels(6);}}
	void process(const ProcessArgs &args) override {

        float cont=params[CONT_PARAM].getValue();
        float cure=params[CURE_PARAM].getValue();
        
        if(lastfixparam<params[FIX_PARAM].getValue() && !group){
            fix=!fix;
            if (fix){lights[FIX_LIGHT].setBrightness(1.f);}
            else{lights[FIX_LIGHT].setBrightness(0.f);}
        }

        if(lastgroupparam<params[GROUP_PARAM].getValue() && !fix){
            group=!group;
            if (group){lights[GROUP_LIGHT].setBrightness(1.f);}
            else{lights[GROUP_LIGHT].setBrightness(0.f);}
        }
        lastfixparam=params[FIX_PARAM].getValue();
        lastgroupparam=params[GROUP_PARAM].getValue();


        for (int i=0; i<16; i++){
            if(fix){

                if(cell[i].fixstate==false){
                        setRed(i);
                }
                else if(cell[i].state==false){
                    setGray(i);}
                else{setGreen(i);}

                if(lastparam[i]<params[CELL_PARAM+i].getValue()){
                    if(cell[i].fixstate==false){
                        setGray(i);
                        cell[i].fixstate=true;
                        cell[i].state=false;
                    }
                    else if(cell[i].state==false){
                        setGreen(i);
                        cell[i].fixstate=true;
                        cell[i].state=true;
                    }
                    else{
                        setRed(i);
                        cell[i].fixstate=false;
                    }
                }          
            }
            else if(group){


                switch(cell[i].group){
                        case 0:
                            setGray(i);

                            break;
                        case 1:
                            setGreen(i);

                            break;
                        case 2:
                            setRed(i);

                            break;
                        case 3:
                            setOrange(i);

                            break;
                    }
                
                if(lastparam[i]<params[CELL_PARAM+i].getValue()){
                    switch(cell[i].group){
                        case 0:
                            setGreen(i);
                            cell[i].group=1;
                            break;
                        case 1:
                            setRed(i);
                            cell[i].group=2;
                            break;
                        case 2:
                            setOrange(i);
                            cell[i].group=3;
                            break;
                        case 3:
                            setGray(i);
                            cell[i].group=0;
                            break;
                    }
                }          










            }
            else{
                if(cell[i].state==false){
                    setGray(i);}
                else{setGreen(i);}
                if(lastparam[i]<params[CELL_PARAM+i].getValue()){
                    if(cell[i].state==false){
                        setGreen(i);
                        cell[i].state=true;
                        cell[i].prevstate=true;
                    }
                    else{
                        setGray(i);
                        cell[i].state=false;
                        cell[i].prevstate=false;
                    }
                
                }
           } 

           lastparam[i]=params[CELL_PARAM+i].getValue();
        }
        
            
            

            
            // Clock stuff   
        if (ClockTrig.process(inputs[CLOCK_INPUT].getVoltage())){
            for (int i=0; i<16; i++){
                if(inputs[GATE_INPUT].getVoltage(i)>0.f){cell[i].state=1.f;}
                int row=i/4;
                int col=i%4;
                //int randcure=rand()%100;
                if(cell[i].prevstate==false){
                    bool x1=false;
                    bool x2=false;
                    bool x3=false;
                    bool x4=false;
                    
                    int randsick1=rand()%100;
                    int randsick2=rand()%100;
                    int randsick3=rand()%100;
                    int randsick4=rand()%100;
                    

                    
                    if(row>0 && cell[(row-1)*4+col].prevstate==true){
                        x1=randsick1<cont;
                    }
                    if(row<3 && cell[(row+1)*4+col].prevstate==true){
                        x2=randsick2<cont;
                    }
                    if(col>0 && cell[row*4+col-1].prevstate==true){
                        x3=randsick3<cont;
                    }
                    if(col<3 && cell[row*4+col+1].prevstate==true){
                        x4=randsick4<cont;
                    }

                    if (x1||x2||x3||x4==true){                   
                        if(!cell[i].fixstate){cell[i].state=true;}
                    }
                }
                else{
                    int randcure=rand()%100;
                    if(randcure<cure){
                      
                        if(!cell[i].fixstate){cell[i].state=false;}
                        
                    }
                }

            }
            bool andGroup[4]={true,true,true,true};
            bool orGroup[4]={false};
            for (int i=0; i<16; i++){
                cell[i].prevstate=cell[i].state;
                if(cell[i].state==true){
                    setGreen(i);
                    outputs[GATE_OUTPUT].setVoltage(10.0f,i);
                    orGroup[cell[i].group]=true;
                }
                else{outputs[GATE_OUTPUT].setVoltage(0.0f,i);setGray(i);andGroup[cell[i].group]=false;}
                    

            }
            for(int i=0;i<4;i++){
                outputs[ORGROUP_OUTPUT+i].setVoltage(orGroup[i] ? 10.f : 0.f);
                outputs[ANDGROUP_OUTPUT+i].setVoltage(andGroup[i] ? 10.f : 0.f);
            }    

            outputs[GATE_OUTPUT].setChannels(16);

       }

       

    }

};





struct CovGateWidget : ModuleWidget {
	CovGateWidget(CovGate* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/CovGate.svg")));
        for(int i=0;i<16;i++){
            addParam(createParam<LEDButton>(mm2px(Vec(30.5+(i%4)*10,30.5+(i/4)*10)), module, CovGate::CELL_PARAM+i));
            addChild(createLight<MLMediumLight<GreenRedLight>>(mm2px(Vec(30+(i%4)*10,30+(i/4)*10)), module, CovGate::CELL_LIGHT+2*i));
        }
        addParam(createParam<RoundBlackKnob>(mm2px(Vec(20,10)), module, CovGate::CONT_PARAM));
        addParam(createParam<RoundBlackKnob>(mm2px(Vec(30,10)), module, CovGate::CURE_PARAM));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10,10)), module, CovGate::CLOCK_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10,30)), module, CovGate::GATE_INPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(100,100)),module,CovGate::GATE_OUTPUT));
        addParam(createParam<LEDButton>(mm2px(Vec(50.5,100.5)), module, CovGate::FIX_PARAM));
        addChild(createLight<MLMediumLight<GreenLight>>(mm2px(Vec(50,100)), module, CovGate::FIX_LIGHT));
        addParam(createParam<LEDButton>(mm2px(Vec(70.5,100.5)), module, CovGate::GROUP_PARAM));
        addChild(createLight<MLMediumLight<GreenLight>>(mm2px(Vec(70,100)), module, CovGate::GROUP_LIGHT));

        for(int i=0;i<4;i++){
            addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(100,30+10*i)), module, CovGate::ORGROUP_OUTPUT+i));
            addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(110,30+10*i)), module, CovGate::ANDGROUP_OUTPUT+i));
        }



	}
};


Model* modelCovGate = createModel<CovGate, CovGateWidget>("CovGate");
