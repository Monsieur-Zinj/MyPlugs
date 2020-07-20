#include "plugin.hpp"


struct ClockDivCvRot : Module {
	enum ParamIds {

		NUM_PARAMS
	};
	enum InputIds {
        CLOCK_INPUT,
        CV_INPUT,
        RESET_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(DIV_OUTPUT,10),
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};
    int count=0;
    rack::dsp::SchmittTrigger  ClockTrig;
    rack::dsp::SchmittTrigger  ResetTrig;
	ClockDivCvRot() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		//configParam(P1_PARAM, 0.f, 1.f, 0.f, "");
		//configParam(P2_PARAM, 0.f, 1.f, 0.f, "");
		//configParam(P3_PARAM, 0.f, 1.f, 0.f, "");
	}

	void process(const ProcessArgs& args) override {

        int cv=((int) (clamp(inputs[CV_INPUT].getVoltage(),0.f,10.f)))%10;

        if (ResetTrig.process(inputs[RESET_INPUT].getVoltage())){
            count=0;
        }
        if (ClockTrig.process(inputs[CLOCK_INPUT].getVoltage())){
            if (count>629){
                count=0;
            }
            for (int i=0;i<10;i++){

                if ((count%((i+1+cv)))==0){
                    outputs[DIV_OUTPUT+i].setVoltage(inputs[CLOCK_INPUT].getVoltage());
                }
                else{outputs[DIV_OUTPUT+i].setVoltage(0.f);}
            }
            count++;
        }
	}
};


struct ClockDivCvRotWidget : ModuleWidget {
	ClockDivCvRotWidget(ClockDivCvRot* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ClockDivCvRot.svg")));
/*
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.875, 35.896)), module, Arp::P1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.875, 54.417)), module, Arp::P2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.875, 72.938)), module, Arp::P3_PARAM));
        */
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10,10)), module, ClockDivCvRot::CLOCK_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10,20)), module, ClockDivCvRot::CV_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10,30)), module, ClockDivCvRot::RESET_INPUT));
        for (int i=0;i<10;i++){
		    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20, 20+i*10)), module, ClockDivCvRot::DIV_OUTPUT+i));
        }


		//addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 108.713)), module, Arp::OUT_OUTPUT));
	}
};


Model* modelClockDivCvRot = createModel<ClockDivCvRot, ClockDivCvRotWidget>("ClockDivCvRot");
