#define TINYOBJLOADER_IMPLEMENTATION

#include "model_loader.hpp"
#include <iostream>

ModelLoader::Model ModelLoader::load_model(const std::string& file_path)
{
    Model                    model;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(file_path, reader_config))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();

    model = process_model(attrib, shapes);

    return model;
}

ModelLoader::Model ModelLoader::process_model(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes)
{
    Model model;

    // Loop over shapes
    for (const auto& shape : shapes)
    {
        // Loop over faces (polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
        {
            size_t fv = shape.mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++)
            {
                // Access to vertex
                const tinyobj::index_t& idx = shape.mesh.indices[index_offset + v];
                const float             vx  = attrib.vertices[3 * idx.vertex_index + 0];
                const float             vy  = attrib.vertices[3 * idx.vertex_index + 1];
                const float             vz  = attrib.vertices[3 * idx.vertex_index + 2];

                // Vertex positions
                model.combined_data.push_back(vx);
                model.combined_data.push_back(vy);
                model.combined_data.push_back(vz);

                // Vertex normals
                if (idx.normal_index >= 0)
                {
                    const float nx = attrib.normals[3 * idx.normal_index + 0];
                    const float ny = attrib.normals[3 * idx.normal_index + 1];
                    const float nz = attrib.normals[3 * idx.normal_index + 2];
                    model.combined_data.push_back(nx);
                    model.combined_data.push_back(ny);
                    model.combined_data.push_back(nz);
                }
                else
                {
                    model.combined_data.insert(model.combined_data.end(), {0.0f, 0.0f, 0.0f});
                }

                // Vertex texcoords
                if (idx.texcoord_index >= 0)
                {
                    const float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                    const float ty = attrib.texcoords[2 * idx.texcoord_index + 1];
                    model.combined_data.push_back(tx);
                    model.combined_data.push_back(ty);
                }
                else
                {
                    model.combined_data.insert(model.combined_data.end(), {0.0f, 0.0f});
                }
            }
            index_offset += fv;
        }
    }

    return model;
}
