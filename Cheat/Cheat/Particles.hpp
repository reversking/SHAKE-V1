#pragma once
#include <vector>
#include <imgui.h>
#include "Brand.hpp"
REVERS_STAMP(Particles_hpp)
#include "GameSDK.hpp"

struct ParticlePixel
{
    float R, G, B, A;
};

namespace particle_colors
{

    ParticlePixel global_to_pixel(const ImVec4& color)
    {
        return { color.x, color.y, color.z, color.w };
    }
}

class ParticleManager
{
public:
    struct Particle
    {
        Vector3 position;
        ImVec4 color;
        float lifetime;
        float size;
        float alpha;
        float rotation;
        int type;
    };

    std::vector<Particle> particles;

    void addSuperParticle(const Vector3& position, const ImVec4& color)
    {

        for (int i = 0; i < 5; i++)
        {
            Particle particle;
            particle.position = position;

            float r = color.x + (((float)rand() / RAND_MAX) * 0.2f - 0.1f);
            float g = color.y + (((float)rand() / RAND_MAX) * 0.2f - 0.1f);
            float b = color.z + (((float)rand() / RAND_MAX) * 0.2f - 0.1f);

            r = r < 0.0f ? 0.0f : (r > 1.0f ? 1.0f : r);
            g = g < 0.0f ? 0.0f : (g > 1.0f ? 1.0f : g);
            b = b < 0.0f ? 0.0f : (b > 1.0f ? 1.0f : b);

            particle.color = ImVec4(r, g, b, 1.0f);
            particle.lifetime = 0.5f + (((float)rand() / RAND_MAX) * 0.5f);
            particle.size = 3.0f + (((float)rand() / RAND_MAX) * 4.0f);
            particle.alpha = 1.0f;
            particle.rotation = ((float)rand() / RAND_MAX) * 360.0f;
            particle.type = rand() % 4;

            particles.push_back(particle);
        }
    }

    void update(float deltaTime)
    {
        for (auto it = particles.begin(); it != particles.end();)
        {
            it->lifetime -= deltaTime;
            it->alpha = it->lifetime / 1.0f;
            it->rotation += deltaTime * 90.0f;
            it->size += deltaTime * 2.0f;

            if (it->lifetime <= 0)
            {
                it = particles.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void render(ImDrawList* drawList, const Matrix& viewMatrix)
    {
        for (const auto& particle : particles)
        {
            Vector2 screenPos;
            if (worldToScreen(viewMatrix, particle.position, screenPos))
            {
                ImColor particleColor = ImColor(
                    particle.color.x,
                    particle.color.y,
                    particle.color.z,
                    particle.alpha
                );

                switch (particle.type)
                {
                case 0:
                    drawList->AddCircleFilled(
                        ImVec2(screenPos.x, screenPos.y),
                        particle.size,
                        particleColor,
                        12
                    );
                    break;

                case 1:
                {
                    const float outerRadius = particle.size;
                    const float innerRadius = particle.size * 0.4f;
                    const int numPoints = 5;
                    const float angleStep = 2.0f * 3.14159f / numPoints;
                    const float rotationRad = particle.rotation * 3.14159f / 180.0f;

                    for (int i = 0; i < numPoints; i++)
                    {
                        float angle1 = rotationRad + i * angleStep;
                        float angle2 = rotationRad + (i + 0.5f) * angleStep;

                        ImVec2 p1(
                            screenPos.x + cosf(angle1) * outerRadius,
                            screenPos.y + sinf(angle1) * outerRadius
                        );
                        ImVec2 p2(
                            screenPos.x + cosf(angle2) * innerRadius,
                            screenPos.y + sinf(angle2) * innerRadius
                        );
                        ImVec2 p3(
                            screenPos.x + cosf(angle1 + angleStep) * outerRadius,
                            screenPos.y + sinf(angle1 + angleStep) * outerRadius
                        );

                        drawList->AddTriangleFilled(p1, p2, p3, particleColor);
                    }
                }
                break;

                case 2:
                {
                    const float halfSize = particle.size * 0.7071f;
                    const float rotationRad = particle.rotation * 3.14159f / 180.0f;
                    const float s = sinf(rotationRad);
                    const float c = cosf(rotationRad);

                    ImVec2 points[4] = {
                        ImVec2(screenPos.x + c * halfSize - s * halfSize, screenPos.y + s * halfSize + c * halfSize),
                        ImVec2(screenPos.x - c * halfSize - s * halfSize, screenPos.y - s * halfSize + c * halfSize),
                        ImVec2(screenPos.x - c * halfSize + s * halfSize, screenPos.y - s * halfSize - c * halfSize),
                        ImVec2(screenPos.x + c * halfSize + s * halfSize, screenPos.y + s * halfSize - c * halfSize)
                    };

                    drawList->AddConvexPolyFilled(points, 4, particleColor);
                }
                break;

                case 3:
                {
                    const float radius = particle.size;
                    const float rotationRad = particle.rotation * 3.14159f / 180.0f;

                    ImVec2 points[3] = {
                        ImVec2(screenPos.x + cosf(rotationRad) * radius,
                               screenPos.y + sinf(rotationRad) * radius),
                        ImVec2(screenPos.x + cosf(rotationRad + 2.0f * 3.14159f / 3.0f) * radius,
                               screenPos.y + sinf(rotationRad + 2.0f * 3.14159f / 3.0f) * radius),
                        ImVec2(screenPos.x + cosf(rotationRad + 4.0f * 3.14159f / 3.0f) * radius,
                               screenPos.y + sinf(rotationRad + 4.0f * 3.14159f / 3.0f) * radius)
                    };

                    drawList->AddTriangleFilled(points[0], points[1], points[2], particleColor);
                }
                break;
                }
            }
        }
    }
};