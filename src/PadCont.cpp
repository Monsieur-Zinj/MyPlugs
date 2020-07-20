/*#include "plugin.hpp"
struct PadCont: Module {

	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
        ENUMS(TRIG_INPUT,3),
        ENUMS(PITCH_INPUT,3),
		NUM_INPUTS
	};
	enum OutputIds {
        ENUMS(GATE_OUTPUT,3),
        ENUMS(PITCH_INPUT,3),
		NUM_OUTPUTS
	};
	enum LightIds {

		NUM_LIGHTS
	};
    float counter;
    rack::dsp::SchmittTrigger  lTrig;



	PadCont() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        }
    
    //void onAdd() override {for (int i=0;i<8;i++){outputs[POLYOUT_OUTPUT+i].setChannels(6);}}
	void process(const ProcessArgs &args) override {

       counter+=args.sampleTime;
       if (counter>0.01){


        
        if (lTrig.process(inputs[LEFT_INPUT].getVoltage())){    
        }

    counter=0;
        }
    }

};




struct PadContWidget : ModuleWidget {
	PadContWidget(PadCont* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/PadContCont.svg")));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(345+20*(i%4),20*(i/4)+55)), module, PadCont::KNOBS1_PARAM+48 +i));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(345+20*(i%4),20*(i/4)+65)), module, PadCont::ALLOUT_OUTPUT +i));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10,85)), module, PadCont::LEFT_INPUT));

	}
};


Model* modelPadCont = createModel<PadCont, PadContWidget>("PadCont");*/
