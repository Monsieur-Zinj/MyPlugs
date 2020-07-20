#include "plugin.hpp"
#include "DynamicComponents.hpp"
struct ArpSeq: Module {

	enum ParamIds {
        DIV_PARAM,
        DEL_PARAM,	
		NUM_PARAMS
        
	};
	enum InputIds {
        ENUMS(CLOCK_INPUT,1),
        ENUMS(CV_INPUT,1),
        ENUMS(GATE_INPUT,1),
        ENUMS(DELNOTE_INPUT,1),
		NUM_INPUTS
	};
	enum OutputIds {
        ENUMS(GATE_OUTPUT,1),
        ENUMS(CV_OUTPUT,1),
        ENUMS(TEST_OUTPUT,1),
		NUM_OUTPUTS
	};
	enum LightIds {
        DEL_LIGHT,
		NUM_LIGHTS
	};

    // Clock stuff 
    int time=0;
    int steptime=0;
    int step=0;
    int inctime = 0;
    int inc=0;
    int stepcount=0;
    int nstep=32;

    // Seq stuff
    int firstgate[1024]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
     //int lastgate[1024]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
        
    float gate[1024]={};
    float cv[1024]={};
    float gatein=0;
    float cvin=0;
    float lastgatein=0;
    int firstinc=-1;
    int lastinc=-1;
    //int gatedur=0;



    // Arp stuff
    bool arp=true;
    bool arpgate[1024]={true};
    float arplenghtscale=0.5;
    float arpsteptime=2.f;
    bool test=false;
    
    rack::dsp::SchmittTrigger  ClockTrig;
    rack::dsp::SchmittTrigger  DelNoteTrig;

    bool isgateraise(float gate,float lastgate){
        if (lastgate<gate){return true;}
        else {return false;}        
    }
    
    bool isgatefall(float gate,float lastgate){
        if (lastgate>gate){return true;}
        else {return false;}        
    }

    void delnote(){
        int i = 0;
        int firstincloc=firstgate[inc];
        
        if (firstgate[inc]>-1){
             while ((firstgate[(firstincloc+i)%1024])==(firstgate[(firstincloc+i+1)%1024]) && i < 1024){
                
                    gate[(firstincloc+i)%1024]=0.f;
                    firstgate[(firstincloc+i)%1024]=-1;
                
                //lastgate[i]=-1;
                    i++;
            }
            gate[(firstincloc+i)%1024]=0.f;
            firstgate[(firstincloc+i)%1024]=-1;
            arpgate[inc]=true;
        }
    }


	ArpSeq() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	
		configParam(DEL_PARAM, 0.0f, 1.0f, 0.0f, "Delete");	
		configParam(DIV_PARAM, -5.f, 5.f, 0.f, "");
        }
    
    //void onAdd() override {for (int i=0;i<8;i++){outputs[POLYOUT_OUTPUT+i].setChannels(6);}}
	void process(const ProcessArgs &args) override {

       //counter+=args.sampleTime;
       //if (counter>0.01){
        gatein=inputs[GATE_INPUT].getVoltage();
        cvin=inputs[CV_INPUT].getVoltage();
        test=false;
        arp=true;


// Div calc
        float div=std::round(params[DIV_PARAM].getValue()); 
        bool neg=(div<0);
        switch ((int)abs(div)) {
              case 0:
                 arp=false;
              case 2:
                div=1.25;
                break;
              case 3:
                div=1.50;
                break;
              case 4:
                div=1.75;
                break;
              case 5:
                div=2;
                break;             
        }
        if (neg){div=1/abs(div);}

        
        // Clock stuff   
        if (ClockTrig.process(inputs[CLOCK_INPUT].getVoltage())){
            steptime=stepcount;
            inctime=steptime/nstep;
            if (div!=0){arpsteptime=abs(nstep/div);}
            
            stepcount=0;
        }
        outputs[TEST_OUTPUT].setVoltage(arpsteptime/20);
       
     
        if (inctime>0){



            if (time%inctime==0){
                inc=(inc+1)%1024;
                if (inc%(1024/nstep)==0){
                    step=step+1;
                    time=0;
                    if ( step>nstep-1){
                        step=0;
                    }
                }
                
                gatein=inputs[GATE_INPUT].getVoltage();

                if (inputs[DELNOTE_INPUT].getVoltage()>0){
                     delnote();
                }
            
                if (isgateraise(gatein,lastgatein)){

                    if (gate[inc]>0){                       
                        delnote();              
                    }                   
                    firstgate[inc]=inc;
                    firstinc=inc;
                }


/*                 if (isgatefall(gatein,lastgatein)){
                    lastgate[firstinc]=inc;

                }
*/

                if (gatein>0){
                    if (gate[inc]>0){
                        
                       delnote();              
                    }
                    firstgate[inc]=firstinc;
                    if(arp==false){
                        

                        arpgate[inc]=false;

                    //gatedur=gatedur+1;
                    }
                    else{   
                        if(arpsteptime>1 && arpsteptime < 1024){
                            if ((inc%(1024/(int)arpsteptime))==0){
                                arpgate[inc]=true;
                                
                            }
                        }
                    }
                    gate[inc]=10.f;
                    cv[inc]=cvin;
                }
                lastgatein=gatein;
            }







       
        }
/*
        if (test){
            outputs[TEST_OUTPUT].setVoltage(5.f);
        }
        else {outputs[TEST_OUTPUT].setVoltage(test);}
*/
          //outputs[GATE_OUTPUT].setVoltage(gate[inc]);
        bool cvchange=false;
        if(arpsteptime>1 && arpsteptime < 1024){
            if(arpgate[inc]==false){outputs[GATE_OUTPUT].setVoltage(gate[inc]);outputs[TEST_OUTPUT].setVoltage(-10.f);}
             else if ((inc%(1024/(int)arpsteptime))<0.5*1024/arpsteptime){
               if ((inc%(1024/(int)arpsteptime)==0)){cvchange=true;outputs[GATE_OUTPUT].setVoltage(10.f);outputs[TEST_OUTPUT].setVoltage(10.f);}
               
               
           }
             else{outputs[GATE_OUTPUT].setVoltage(0.f);} 
        
        }
        outputs[TEST_OUTPUT].setVoltage(div);
        if (arpgate[inc]==false){
            if (gate[inc]>0) {outputs[CV_OUTPUT].setVoltage(cv[inc]);}
        }
        else{
            if (gate[inc]>0 && cvchange==true) {outputs[CV_OUTPUT].setVoltage(cv[inc]);}


        }
        if(inputs[CLOCK_INPUT].isConnected()){
            time=time+1;
            stepcount=stepcount+1;
        }
        // lastgatein=gatein;  





    //counter=0;
        //}
    }

};

template <typename BASE>
struct MLMediumLight : BASE {
	MLMediumLight() {
		this->box.size = Vec(22.0, 22.0);
	}
};



struct ArpSeqWidget : ModuleWidget {
	ArpSeqWidget(ArpSeq* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ArpSeq.svg")));
        //addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(345+20*(i%4),20*(i/4)+55)), module, ArpSeq::KNOBS1_PARAM+48 +i));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50,20)), module, ArpSeq::GATE_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50,30)), module, ArpSeq::CV_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50,50)), module, ArpSeq::TEST_OUTPUT));

        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30, 20)), module, ArpSeq::DIV_PARAM));



        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10,10)), module, ArpSeq::CLOCK_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10,20)), module, ArpSeq::GATE_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10,30)), module, ArpSeq::CV_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10,40)), module, ArpSeq::DELNOTE_INPUT));

        addParam(createParam<LEDButton>(Vec(100,100), module, ArpSeq::DEL_PARAM));
        addChild(createLight<MLMediumLight<GreenLight>>(Vec(100,120), module, ArpSeq::DEL_LIGHT));


	}
};


Model* modelArpSeq = createModel<ArpSeq, ArpSeqWidget>("ArpSeq");
