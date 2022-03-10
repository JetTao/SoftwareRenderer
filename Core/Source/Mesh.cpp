#include "../Include/Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

static void PrintInfo(const tinyobj::attrib_t& attrib,
                      const std::vector<tinyobj::shape_t>& shapes,
                      const std::vector<tinyobj::material_t>& materials,
                      bool triangulate = true) {
  std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
  std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
  std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
            << std::endl;

  std::cout << "# of shapes    : " << shapes.size() << std::endl;
  std::cout << "# of materials : " << materials.size() << std::endl;

  for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
    printf("  v[%ld] = (%f, %f, %f)\n", v,
           static_cast<const double>(attrib.vertices[3 * v + 0]),
           static_cast<const double>(attrib.vertices[3 * v + 1]),
           static_cast<const double>(attrib.vertices[3 * v + 2]));
  }

  for (size_t v = 0; v < attrib.normals.size() / 3; v++) {
    printf("  n[%ld] = (%f, %f, %f)\n", v,
           static_cast<const double>(attrib.normals[3 * v + 0]),
           static_cast<const double>(attrib.normals[3 * v + 1]),
           static_cast<const double>(attrib.normals[3 * v + 2]));
  }

  for (size_t v = 0; v < attrib.texcoords.size() / 2; v++) {
    printf("  uv[%ld] = (%f, %f)\n", v,
           static_cast<const double>(attrib.texcoords[2 * v + 0]),
           static_cast<const double>(attrib.texcoords[2 * v + 1]));
  }

  for (size_t i = 0; i < shapes.size(); i++) {
    printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
    printf("Size of shape[%ld].indices: %ld\n", i,
           shapes[i].mesh.indices.size());

    if (triangulate) {
      printf("Size of shape[%ld].material_ids: %ld\n", i,
             shapes[i].mesh.material_ids.size());
      assert((shapes[i].mesh.indices.size() % 3) == 0);
      for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
        tinyobj::index_t i0 = shapes[i].mesh.indices[3 * f + 0];
        tinyobj::index_t i1 = shapes[i].mesh.indices[3 * f + 1];
        tinyobj::index_t i2 = shapes[i].mesh.indices[3 * f + 2];
        printf("  idx[%ld] = %d/%d/%d, %d/%d/%d, %d/%d/%d. mat_id = %d\n", f,
               i0.vertex_index, i0.normal_index, i0.texcoord_index,
               i1.vertex_index, i1.normal_index, i1.texcoord_index,
               i2.vertex_index, i2.normal_index, i2.texcoord_index,
               shapes[i].mesh.material_ids[f]);
      }
    } else {
      for (size_t f = 0; f < shapes[i].mesh.indices.size(); f++) {
        tinyobj::index_t idx = shapes[i].mesh.indices[f];
        printf("  idx[%ld] = %d/%d/%d\n", f, idx.vertex_index, idx.normal_index,
               idx.texcoord_index);
      }

      printf("Size of shape[%ld].material_ids: %ld\n", i,
             shapes[i].mesh.material_ids.size());
      assert(shapes[i].mesh.material_ids.size() ==
             shapes[i].mesh.num_face_vertices.size());
      for (size_t m = 0; m < shapes[i].mesh.material_ids.size(); m++) {
        printf("  material_id[%ld] = %d\n", m, shapes[i].mesh.material_ids[m]);
      }
    }

    printf("shape[%ld].num_faces: %ld\n", i,
           shapes[i].mesh.num_face_vertices.size());
    for (size_t v = 0; v < shapes[i].mesh.num_face_vertices.size(); v++) {
      printf("  num_vertices[%ld] = %ld\n", v,
             static_cast<long>(shapes[i].mesh.num_face_vertices[v]));
    }

    // printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
    // assert((shapes[i].mesh.positions.size() % 3) == 0);
    // for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
    //  printf("  v[%ld] = (%f, %f, %f)\n", v,
    //    static_cast<const double>(shapes[i].mesh.positions[3*v+0]),
    //    static_cast<const double>(shapes[i].mesh.positions[3*v+1]),
    //    static_cast<const double>(shapes[i].mesh.positions[3*v+2]));
    //}

    printf("shape[%ld].num_tags: %ld\n", i, shapes[i].mesh.tags.size());
    for (size_t t = 0; t < shapes[i].mesh.tags.size(); t++) {
      printf("  tag[%ld] = %s ", t, shapes[i].mesh.tags[t].name.c_str());
      printf(" ints: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].intValues.size(); ++j) {
        printf("%ld", static_cast<long>(shapes[i].mesh.tags[t].intValues[j]));
        if (j < (shapes[i].mesh.tags[t].intValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");

      printf(" floats: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].floatValues.size(); ++j) {
        printf("%f", static_cast<const double>(
                         shapes[i].mesh.tags[t].floatValues[j]));
        if (j < (shapes[i].mesh.tags[t].floatValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");

      printf(" strings: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].stringValues.size(); ++j) {
        printf("%s", shapes[i].mesh.tags[t].stringValues[j].c_str());
        if (j < (shapes[i].mesh.tags[t].stringValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");
      printf("\n");
    }
  }

  for (size_t i = 0; i < materials.size(); i++) {
    printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
    printf("  material.Ka = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].ambient[0]),
           static_cast<const double>(materials[i].ambient[1]),
           static_cast<const double>(materials[i].ambient[2]));
    printf("  material.Kd = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].diffuse[0]),
           static_cast<const double>(materials[i].diffuse[1]),
           static_cast<const double>(materials[i].diffuse[2]));
    printf("  material.Ks = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].specular[0]),
           static_cast<const double>(materials[i].specular[1]),
           static_cast<const double>(materials[i].specular[2]));
    printf("  material.Tr = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].transmittance[0]),
           static_cast<const double>(materials[i].transmittance[1]),
           static_cast<const double>(materials[i].transmittance[2]));
    printf("  material.Ke = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].emission[0]),
           static_cast<const double>(materials[i].emission[1]),
           static_cast<const double>(materials[i].emission[2]));
    printf("  material.Ns = %f\n",
           static_cast<const double>(materials[i].shininess));
    printf("  material.Ni = %f\n", static_cast<const double>(materials[i].ior));
    printf("  material.dissolve = %f\n",
           static_cast<const double>(materials[i].dissolve));
    printf("  material.illum = %d\n", materials[i].illum);
    printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
    printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
    printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
    printf("  material.map_Ns = %s\n",
           materials[i].specular_highlight_texname.c_str());
    printf("  material.map_bump = %s\n", materials[i].bump_texname.c_str());
    printf("  material.map_d = %s\n", materials[i].alpha_texname.c_str());
    printf("  material.disp = %s\n", materials[i].displacement_texname.c_str());
    printf("  material.refl = %s\n", materials[i].reflection_texname.c_str());
    std::map<std::string, std::string>::const_iterator it(
        materials[i].unknown_parameter.begin());
    std::map<std::string, std::string>::const_iterator itEnd(
        materials[i].unknown_parameter.end());

    for (; it != itEnd; it++) {
      printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
    }
    printf("\n");
  }
}

Mesh::Mesh(const fs::path& path)
{
    LoadMeshFromPath(path);
}

bool Mesh::LoadMeshFromPath(const fs::path& path)
{
    using namespace tinyobj;
    ObjReaderConfig readerConfig;
    readerConfig.triangulate = true;
    readerConfig.mtl_search_path = path.parent_path().string();

    ObjReader reader;
    if (!reader.ParseFromFile(path.string(), readerConfig))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error(); 
        }
        return false;
    }

    if(!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning();
    }



    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    // PrintInfo(attrib, shapes, reader.GetMaterials());

    for (auto i = 0; i < shapes.size(); i++)
    {
        auto indices = shapes[i].mesh.indices;
        for (auto j = 0; j < indices.size() / 3; j++)
        {
            for (auto k = 0; k < 3; k++)    
            {
                mIndices.emplace_back(
                    indices[3 * j + k].vertex_index,
                    indices[3 * j + k].texcoord_index,
                    indices[3 * j + k].normal_index
                );
            }
        
        }
    }

    for (auto i = 0; i < attrib.vertices.size() / 3; i++)
    {
        mVertices.emplace_back(
            attrib.vertices[3 * i + 0],
            attrib.vertices[3 * i + 1],
            attrib.vertices[3 * i + 2]
        );
    }

    for (auto i = 0; i < attrib.texcoords.size() / 2; i++)
    {
      float u = attrib.texcoords[2 * i + 0];
      float v = attrib.texcoords[2 * i + 1];
        if (u > 1.0f) {
            u -= std::floor(u);
        }
        if (v > 1.0f) {
            v -= std::floor(v);
        }
        mTexCoords.emplace_back(u, v);
    }
        
    for (auto i = 0; i < attrib.normals.size() / 3; i++)
    {
        mNormals.emplace_back(
            attrib.normals[3 * i + 0],
            attrib.normals[3 * i + 1],
            attrib.normals[3 * i + 2]
        );
    }

    std::cout << "model_name: " + path.stem().string() << "\nvertex_size: " 
        << mVertices.size() << "\nface_size: " << mIndices.size() / 3 << std::endl;;

    return true;
}

