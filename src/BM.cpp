#include "plugin.hpp"


struct BM : Module {
	
	float phase1=0.f;
	float phase2=0.f;
	float phase=0.f;
    float prevres=0;

	enum ParamIds {
		D_PARAM,
		D2_PARAM,
		D1_PARAM,
		F2_PARAM,
		F_PARAM,
		F1_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		CLOCK_INPUT,
		RESET_INPUT,
		CLOCK2_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT_OUTPUT,
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	BM() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(D_PARAM, -1.f, 1.f, 0.f, "");
		configParam(D2_PARAM, -1.f, 1.f, 0.f, "");
		configParam(D1_PARAM, -1.f, 1.f, 0.f, "");
		configParam(F2_PARAM, -4.f, 4.f, 0.f, "","",2,1);
		configParam(F_PARAM, -4.f, 4.f, 0.f, "","",2,1);
		configParam(F1_PARAM, -4.f, 4.f, 0.f, "","",2,1);
	}

	void process(const ProcessArgs& args) override {
		float f = params[F_PARAM].getValue();
		float f1 = params[F1_PARAM].getValue();
		float f2 = params[F2_PARAM].getValue();
		float d = params[D_PARAM].getValue();
		float d1 = params[D1_PARAM].getValue();
		float d2 = params[D2_PARAM].getValue();
		float trig = inputs[CLOCK_INPUT].getVoltage();
		float trig2 = inputs[CLOCK2_INPUT].getVoltage();
		float res = inputs[RESET_INPUT].getVoltage();
		//float out=0.f;
		//float out1=0.f;
		//float out2=0.f;
		float lfo=0;
		float lfo1=0;
		float lfo2=0;



		phase += f * args.sampleTime;
		phase1 += f1 * args.sampleTime;
		phase2 += f2 * args.sampleTime;

		if (prevres - res < 0){
			phase = 0.f;
			phase1 = 0.f;
			phase2 = 0.f;
		}
		prevres=res;

		if (phase >= 1)
			phase -= 1.f;

		if (phase1 >= 1)
			phase1 -= 1.f;

		if (phase2 >= 1)
			phase2 -= 1.f;

		
		if (phase < d )
			lfo=1.f;
	
		if (phase1 < d1 )
			lfo1=1.f;

		if (phase2 < d2 )
			lfo2=1.f;
		
		

		outputs[OUT1_OUTPUT].setVoltage(10.f * simd::ifelse((lfo1==1.f) && (trig>0.f) && (lfo==1.f), 1.f, 0.f));
		outputs[OUT2_OUTPUT].setVoltage(10.f * simd::ifelse((lfo2==1.f) && (trig2>0.f) && (lfo==0.f), 1.f, 0.f));

        outputs[OUT_OUTPUT].setVoltage(10.f * simd::ifelse((lfo2==0.f) && (lfo2==0.f), 1.f, 0.f));
	
        
	}
};


struct BMWidget : ModuleWidget {
	BMWidget(BM* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/BM.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(37.15, 15.178)), module, BM::D_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(68.688, 15.712)), module, BM::D2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(9.889, 15.98)), module, BM::D1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(67.886, 46.983)), module, BM::F2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(36.616, 47.785)), module, BM::F_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(10.424, 48.319)), module, BM::F1_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(9.755, 73.309)), module, BM::CLOCK_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(31.538, 84.401)), module, BM::RESET_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(9.889, 92.953)), module, BM::CLOCK2_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(37.15, 112.731)), module, BM::OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(9.354, 112.998)), module, BM::OUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(67.619, 113.8)), module, BM::OUT2_OUTPUT));
	}
};


Model* modelBM = createModel<BM, BMWidget>("BM");
