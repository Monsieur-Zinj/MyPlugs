#include "plugin.hpp"


Plugin* pluginInstance;


void init(Plugin* p) {
	pluginInstance = p;
	p->addModel(modelMymodule);
	p->addModel(modelBM);
	p->addModel(modelCMul);
	p->addModel(modelSixMix);
	p->addModel(modelArp);
    p->addModel(modelVolDrumCont);
    p->addModel(modelArpSeq);
    p->addModel(modelCovGate);
    p->addModel(modelSelGate);
    p->addModel(modelClockDivCvRot);






	// Add modules here
	// p->addModel(modelMyModule);

	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}
