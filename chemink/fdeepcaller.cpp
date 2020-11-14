#include "chemink.h"
#include "fdeepcaller.h"

using namespace fdeep;

#include <thread>
#include <stdexcept>

using namespace std;

FdeepCaller::FdeepCaller()
        : loaded(false) {
    kerasModel = (model *) calloc(1, sizeof(model));
    if (!kerasModel) {
        throwErrorBox("内存分配失败", "在FdeepCaller::FdeepCaller中");
        exit(-1);
    }
}

FdeepCaller::~FdeepCaller() {
    delete kerasModel;
}


bool FdeepCaller::isLoaded() {
    return loaded;
}

void FdeepCaller::loadModel(const char *content) {
    try {
        *kerasModel = read_model_from_string(content);
    }
    catch (runtime_error err) {
        throwErrorBox("模型加载失败", err.what());
        loaded = false;
    }
    loaded = true;
}


void FdeepCaller::predicts(fdeep::tensor5s_vec &_input, fdeep::tensor5s_vec &_output) {
    _output = kerasModel->predict_multi(_input, true);
}

