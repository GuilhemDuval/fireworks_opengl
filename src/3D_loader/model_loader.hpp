#pragma once

#include <string>
#include <vector>
#include "tiny_obj_loader.h"

class ModelLoader {
public:
    struct Model {
        std::vector<float> combined_data;
    };

    static Model load_model(const std::string& file_path);

private:
    static Model process_model(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes);
};
