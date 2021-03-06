#include "plugin.hpp"


struct SixMix : Module {
	enum ParamIds {
		MUL1_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		IN1_INPUT,
		IN2_INPUT,
		IN3_INPUT,
		IN4_INPUT,
		IN5_INPUT,
		IN6_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};
	float out = 0.f;

	SixMix() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(MUL1_PARAM, 0.f, 6.f, 0.f, "");
	}

	void process(const ProcessArgs& args) override {
		float mix=params[MUL1_PARAM].getValue();
		float smix=eucMod(mix,1.f);
		if ( 0<=mix && mix<1 ){
			out=smix*inputs[IN2_INPUT].getVoltage()+(1-smix)*inputs[IN1_INPUT].getVoltage();
		}
		if ( 1<=mix && mix<2 ){
			out=smix*inputs[IN3_INPUT].getVoltage()+(1-smix)*inputs[IN2_INPUT].getVoltage();
		}
		if ( 2<=mix && mix<3 ){
			out=smix*inputs[IN4_INPUT].getVoltage()+(1-smix)*inputs[IN3_INPUT].getVoltage();
		}
		if ( 3<=mix && mix<4 ){
			out=smix*inputs[IN5_INPUT].getVoltage()+(1-smix)*inputs[IN4_INPUT].getVoltage();
		}
		if ( 4<=mix && mix<5 ){
			out=smix*inputs[IN6_INPUT].getVoltage()+(1-smix)*inputs[IN5_INPUT].getVoltage();
		}
		outputs[OUT_OUTPUT].setVoltage(out);

			
		
	}
};


struct SixMixWidget : ModuleWidget {
	SixMixWidget(SixMix* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/SixMix.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(14.552, 92.781)), module, SixMix::MUL1_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.552, 9.438)), module, SixMix::IN1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.552, 21.344)), module, SixMix::IN2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.552, 33.25)), module, SixMix::IN3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.552, 45.156)), module, SixMix::IN4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.552, 57.062)), module, SixMix::IN5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.552, 68.969)), module, SixMix::IN6_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.552, 108.656)), module, SixMix::OUT_OUTPUT));
	}
};


Model* modelSixMix = createModel<SixMix, SixMixWidget>("SixMix");
