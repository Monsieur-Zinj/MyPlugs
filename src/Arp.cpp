#include "plugin.hpp"


struct Arp : Module {
	enum ParamIds {
		P1_PARAM,
		P2_PARAM,
		P3_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		CL_INPUT,
		PITCH_INPUT,
		MOD_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Arp() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(P1_PARAM, 0.f, 1.f, 0.f, "");
		configParam(P2_PARAM, 0.f, 1.f, 0.f, "");
		configParam(P3_PARAM, 0.f, 1.f, 0.f, "");
	}

	void process(const ProcessArgs& args) override {
	}
};


struct ArpWidget : ModuleWidget {
	ArpWidget(Arp* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Arp.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.875, 35.896)), module, Arp::P1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.875, 54.417)), module, Arp::P2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.875, 72.938)), module, Arp::P3_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.118, 15.118)), module, Arp::CL_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.292, 17.375)), module, Arp::PITCH_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(25.135, 17.375)), module, Arp::MOD_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 108.713)), module, Arp::OUT_OUTPUT));
	}
};


Model* modelArp = createModel<Arp, ArpWidget>("Arp");