#pragma once
#ifndef __SCENE_H
#define __SCENE_H

#include "renderer/core/fwd.h"
#include "renderer/core/geometry.h"
#include "renderer/core/primitive.h"
#include "renderer/core/interaction.h"

#include <vector>


class Scene {
public:
    Scene() {}

    bool Intersect(const Ray& ray) const;
    bool IntersectP(const Ray& ray, Interaction* interaction) const;
    Spectrum Shading(const Interaction& interaction) const;

    int AddTriangleMesh(TriangleMesh triangleMesh);
    std::pair<int, int> AddTriangles(std::vector<std::shared_ptr<Triangle>> triangles);
    int AddMaterial(std::shared_ptr<Material> material);
    int AddLight(std::shared_ptr<Light> light);
    void AddPrimitive(Primitive p);

    std::vector<TriangleMesh> m_triangleMeshes;
    std::vector<Triangle> m_triangles;
    std::vector<Material> m_materials;
    std::vector<Light> m_lights;
    std::vector<Primitive> m_primitives;
};

inline 
bool Scene::Intersect(const Ray& ray) const
{
    for (int i = 0; i < m_primitives.size(); i++) {
        int triangleID = m_primitives[i].m_shapeID;
        bool hit = m_triangles[triangleID].Intersect(ray);
        if (hit) {
            return true;
        }
    }
    return false;
}

inline
bool Scene::IntersectP(const Ray& ray, Interaction* interaction) const
{
    Float tHit;
    bool ret_hit = false;
    for (int i = 0; i < m_primitives.size(); i++) {
        int triangleID = m_primitives[i].m_shapeID;
        bool hit = m_triangles[triangleID].IntersectP(ray, &tHit, interaction);
        if (hit) {
            ret_hit = true;
            ray.tMax = tHit;
            interaction->m_primitiveID = i;
        }
    }
    return ret_hit;
}

inline 
Spectrum Scene::Shading(const Interaction& interaction) const
{
    const int& primID = interaction.m_primitiveID;
    const int& mtlID = m_primitives[primID].m_materialID;
    return m_materials[mtlID].m_Kd;
}


#endif // !__SCENE_H
