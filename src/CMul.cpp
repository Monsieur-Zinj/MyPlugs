#include "plugin.hpp"

   
struct CMul : Module {
	enum ParamIds {
		MUL1_PARAM,
		MUL2_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		CV1_INPUT,
		IN1_INPUT,
		CV2_INPUT,
		IN2_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

    int i1=0;
    int i2=0;
    float j=0;
    float j2=0;   
    float time1=0;
    float time2=0;
    float lastin1=0;
    float lastin2=0;


	CMul() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(MUL1_PARAM, 2.f, 10.f, 2.f, "");
		configParam(MUL2_PARAM, 2.f, 10.f, 2.f, "");
	}

	void process(const ProcessArgs& args) override { 
        float cv1 = inputs[CV1_INPUT].getVoltage();
        float cv2 = inputs[CV2_INPUT].getVoltage();
        float mul1 = std::floor(params[MUL1_PARAM].getValue())+cv1;
        float mul2 = std::floor(params[MUL2_PARAM].getValue())+cv2;
   		float in1 = inputs[IN1_INPUT].getVoltage();
		float in2 = inputs[IN2_INPUT].getVoltage();




        if (lastin1>in1) {
            time1=i1;
            i1=0;
            j=0.f;
        }

        if (lastin2>in2) {
            time2=i2;
            j2=0.f;
            i2=0;
        }




        if (std::floor(time1/mul1)>0 && eucMod(j,std::floor(time1/mul1)) == 0) {
            outputs[OUT1_OUTPUT].setVoltage(10.f);
            j=0;
        }
        else {
            outputs[OUT1_OUTPUT].setVoltage(0.f);
        }

        if (std::floor(time2/mul2)>0 && eucMod(j2,std::floor(time2/mul2)) == 0) {
            outputs[OUT2_OUTPUT].setVoltage(10.f);
            j2=0;
        }
        else {
            outputs[OUT2_OUTPUT].setVoltage(0.f);
        }

        lastin1=in1;
        lastin2=in2;
        i1=i1+1;
        j=j+1;
        i2=i2+1;
        j2=j2+1;
        
	}
};


struct CMulWidget : ModuleWidget {
	CMulWidget(CMul* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/CMul.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.234, 36.426)), module, CMul::MUL1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.208, 95.975)), module, CMul::MUL2_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.502, 19.454)), module, CMul::CV1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.42, 51.82)), module, CMul::IN1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.234, 76.65)), module, CMul::CV2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.216, 113.929)), module, CMul::IN2_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(24.322, 51.526)), module, CMul::OUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(24.027, 113.614)), module, CMul::OUT2_OUTPUT));
	}
};


Model* modelCMul = createModel<CMul, CMulWidget>("CMul");
