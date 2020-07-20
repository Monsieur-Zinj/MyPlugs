#include "plugin.hpp"
struct VolDrumCont : Module {

	enum ParamIds {
        ENUMS(KNOBS1_PARAM,56),
		NUM_PARAMS
	};
	enum InputIds {
        ENUMS(IN_INPUT,8),
        LEFT_INPUT,
        RIGHT_INPUT,
        UP_INPUT,
        DOWN_INPUT,
        ALL_INPUT,

		NUM_INPUTS
	};
	enum OutputIds {
        ENUMS(ALLOUT_OUTPUT,8),
        ENUMS(POLYOUT_OUTPUT,8),
		NUM_OUTPUTS
	};
	enum LightIds {

		NUM_LIGHTS
	};
    float counter;
    int state=0;
    bool all=false;
    bool init=false;
    Vec pos=Vec(0,0);
    rack::dsp::SchmittTrigger  lTrig;
    rack::dsp::SchmittTrigger  rTrig;
    rack::dsp::SchmittTrigger  uTrig;
    rack::dsp::SchmittTrigger  dTrig;
    rack::dsp::SchmittTrigger  allTrig;
    float prevVal[8];


	VolDrumCont() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        for (int i=0;i<56;i++){
            configParam(KNOBS1_PARAM+i, 0.f, 10.f, 0.f);
        }

	}
    
    Vec StateToVec(int state){
        //state=state%6;
        return Vec(state%3,state/3);
    }

    //Vec(70+95*(state%3),50+65*(state/3))

    int VecToState(Vec pos){
        return pos.y*3+pos.x;

    }

    Vec PosRect(){

        if (all){return(Vec(1095,300));}
        else{ return Vec(3*(73.5+95*(state%3)),3*(45+65*(state/3)));}
        //else{ return Vec(70,70);}
    }
    
    //void onAdd() override {for (int i=0;i<8;i++){outputs[POLYOUT_OUTPUT+i].setChannels(6);}}
	void process(const ProcessArgs &args) override {

       counter+=args.sampleTime;
       if (counter>0.01){
       if (init!=true){for (int i=0;i<8;i++){outputs[POLYOUT_OUTPUT+i].setChannels(6);}}

        
            if (lTrig.process(inputs[LEFT_INPUT].getVoltage())){
                pos.x=int((pos.x-1))%3;
                if (pos.x<0){pos.x=pos.x+3;}
                state=VecToState(pos);        
            }
            if (rTrig.process(inputs[RIGHT_INPUT].getVoltage())){
                pos.x=int((pos.x+1))%3;
                state=VecToState(pos);        
            }
            if (dTrig.process(inputs[DOWN_INPUT].getVoltage())){
                pos.y=int((pos.y-1))%2;
                if (pos.y<0){pos.y=pos.y+2;}
                state=VecToState(pos);        
            }
            if (uTrig.process(inputs[UP_INPUT].getVoltage())){
                pos.y=int((pos.y+1))%2;
                state=VecToState(pos);        
            }

            if (allTrig.process(inputs[ALL_INPUT].getVoltage())){
                all=!all;      
            } 
            if (all){
                //outputs[ALLOUT_OUTPUT].setVoltage(inputs[IN_INPUT].getVoltage());
                state=6;
            }
            else {state=VecToState(pos);}
           


            //float test=inputs[IN_INPUT].getVoltage();
            //params[KNOBS1_PARAM].setValue(inputs[IN_INPUT].getVoltage());
            //params[KNOBS1_PARAM+1].setValue(pos.x);
            //params[KNOBS1_PARAM+2].setValue(pos.y);
            float value[8];
            
            for (int i=0;i<8;i++){
                   value[i]=inputs[IN_INPUT+i].getVoltage();
                    if ( value[i]!=prevVal[i] ){
                        params[KNOBS1_PARAM+i+8*state].setValue(value[i]);
                        if(all){outputs[ALLOUT_OUTPUT+i].setVoltage(value[i]);}
                        else{outputs[POLYOUT_OUTPUT+i].setVoltage(value[i],state);}
                        prevVal[i]=value[i];
                    }
                    //outputs[POLYOUT_OUTPUT+state].setChannels(8);
                    
            }
            counter=0;
        }
    }

};

struct VolDrumContDisplay: TransparentWidget{
    VolDrumCont *module;
    VolDrumContDisplay(){

    }

    
    void draw(const DrawArgs &args) override{
        if (module){box.pos=module->PosRect();}
        else{box.pos=Vec(70,70);};
        nvgBeginPath(args.vg);
        nvgRect(args.vg, 0,0, 30,30);
        nvgFillColor(args.vg, nvgRGBA(255,192,0,255));
        nvgFill(args.vg);
    }
};


struct VolDrumContWidget : ModuleWidget {
	VolDrumContWidget(VolDrumCont* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/VolDrumCont.svg")));

        for (int i=0;i<8;i++){
            addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10+10*(i%2),10*(i/2)+10)), module, VolDrumCont::IN_INPUT +i));
            for(int j=0;j<6;j++){
            
                addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec((50+95*(j%3))+20*(i%4),20*(i/4)+(10+65*(j/3)))), module, VolDrumCont::KNOBS1_PARAM+j*8+i));
            }
            addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(345+20*(i%4),20*(i/4)+55)), module, VolDrumCont::KNOBS1_PARAM+48 +i));
            addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(345+20*(i%4),20*(i/4)+65)), module, VolDrumCont::ALLOUT_OUTPUT +i));
            addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(345+20*(i%4),10*(i/4)+10)), module, VolDrumCont::POLYOUT_OUTPUT +i));
        }
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10,85)), module, VolDrumCont::LEFT_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(20,85)), module, VolDrumCont::RIGHT_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15,92)), module, VolDrumCont::DOWN_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15,78)), module, VolDrumCont::UP_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15,110)), module, VolDrumCont::ALL_INPUT));

        VolDrumContDisplay *display = new VolDrumContDisplay();
        //display->box.pos = module->PosRect();
		display->box.size = Vec(300, 120);// 3 characters
        display->module = module;
		addChild(display);	

	}
};


Model* modelVolDrumCont = createModel<VolDrumCont, VolDrumContWidget>("VolDrumCont");
