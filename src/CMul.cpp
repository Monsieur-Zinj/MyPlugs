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

	CMul() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(MUL1_PARAM, 0.f, 1.f, 0.f, "");
		configParam(MUL2_PARAM, 0.f, 1.f, 0.f, "");
	}

	void process(const ProcessArgs& args) override {
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