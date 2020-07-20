#include "plugin.hpp"


/*
struct sixposknob : RoundBlackKnob {
	sixposknob() {
		this->speed=1.3f;
        this->minAngle = -0.4*3.14;
		this->maxAngle = 0.4*3.14;
        this->snap=true;
	}
};
*/







struct SelGate : Module {

	enum ParamIds {
        TEST_PARAM,
		ENUMS(SEL_PARAM,4),
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(GATE_INPUT,16),
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(GATE_OUTPUT,4),
		NUM_OUTPUTS
	};
	enum LightIds {
		ENUMS(INPUT_LIGHT,16),
		NUM_LIGHTS
	};

    float lastparam[4]={0};
    
    int selstate[4]={0};


    void SetGreen(int j, int i){
        lights[INPUT_LIGHT+j*4+i].setBrightness(1.f);
    }
    void SetGray(int j, int i){
        lights[INPUT_LIGHT+j*4+i].setBrightness(0.f);
    }

	SelGate() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}





	void process(const ProcessArgs &args) override {

        bool state[4][4]={{false,false,false,false},{false,false,false,false},{false,false,false,false},{false,false,false,false}};
        for (int j=0;j<4;j++){
        
            if(lastparam[j]<params[SEL_PARAM+j].getValue()){
                selstate[j]=(selstate[j]+1)%5;
            }
            lastparam[j]=params[SEL_PARAM+j].getValue();
            

            for (int i=0;i<4;i++){

                    for (int i=0;i<selstate[j];i++){
                        state[j][i]=true;

                    }

            }

            
                
                
            bool gateout=true;
            for(int i=0;i<4;i++){
                
                if(state[j][i]){    
                    SetGreen(j,i);
                    if(inputs[GATE_INPUT+j*4+i].getVoltage()<0.1){gateout=false;}
                                        
                }
                else{
                    SetGray(j,i);
                    if(inputs[GATE_INPUT+j*4+i].getVoltage()>0){gateout=false;}
                }
		    }
            outputs[GATE_OUTPUT+j].setVoltage( gateout ? 10.f: 0.f );
        }   
    }
};


struct SelGateWidget : ModuleWidget {
	SelGateWidget(SelGate* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/SelGate.svg")));

		
        for (int j=0;j<4;j++){
            addParam(createParamCentered<LEDButton>(mm2px(Vec(10+(j%2)*15, 10+60*(j/2))), module, SelGate::SEL_PARAM+j));
            for (int i=0;i<4;i++){
		        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10+(j%2)*15, 20+60*(j/2)+i*10)), module, SelGate::GATE_INPUT+j*4+i));
                addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(3+(j%2)*15, 20+60*(j/2)+i*10)), module, SelGate::INPUT_LIGHT+j*4+i));
            }
            addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(10+(j%2)*15, 60+60*(j/2))), module, SelGate::GATE_OUTPUT+j));
        }
		//addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(10, 60)), module, SelGate::GATE_OUTPUT+j));
        //addParam(createParamCentered<sixposknob>(Vec(50,50),module, SelGate::TEST_PARAM));

    }
};


Model* modelSelGate = createModel<SelGate, SelGateWidget>("SelGate");
