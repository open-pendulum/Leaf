#pragma once
#include "Layer.h"

namespace Leaf {
class LayerStack {
public:
    LayerStack();
    ~LayerStack();
    void PushLayer(Layer *layer);
    void PushOverlay(Layer *overlay);
    void PopLayer(Layer *layer);
    void PopOverlay(Layer *overlay);
    std::vector<Layer *>::iterator begin() {
        return mLayers.begin();
    }
    std::vector<Layer *>::iterator end() {
        return mLayers.end();
    }

private:
    std::vector<Layer *> mLayers;
    unsigned int mLayerInsertIndex = 0;
};
}  // namespace Leaf