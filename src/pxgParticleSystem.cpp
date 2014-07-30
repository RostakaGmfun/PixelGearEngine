/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgParticleSystem.h>
#include <pxgTools.h>
#include <pxgShader.h>
#include <pxgScene.h>

#include <glm/glm.hpp>

using namespace glm;

#define SHADER(s) #s

pxgParticleSystem::pxgParticleSystem(): pxgNode(), updateShader(NULL), renderShader(NULL), scene(NULL), texture(NULL),
    minSize(1), maxSize(1), minRot(vec3(0,0,0)), maxRot(vec3(0,0,0)), minStartVel(vec3(0,1,0)), maxStartVel(vec3(0,1,0)),
    minLifetime(100), maxLifetime(100), spawnRadius(0), minAcceleration(vec3(0,0.05f,0)), maxAcceleration(vec3(0,0.1f,0)),
    spawnRate(1), maxParticles(0)
{

}

std::string pxgParticleSystem::pxgDefaultUpdateFunc =
        SHADER(
            void pxgParticleUpdateFunc()
            {
                float _age = Age[0]+1;
                if(Type[0]==0) //emitter particle
                {
                    int numV = numVertsPerStep;
                    if(numV>MAX_VERTS)
                        numV = MAX_VERTS;
                    for(int i = 0;i<numV; i++)
                    {
                        type = 1; //normal particle
                        position = Position[0];
                        velocity = uVelocity;
                        acceleration = uAcceleration;
                        color = uColor;
                        size = uSize;
                        age = 0;
                        lifetime = uLifetime;
                        EmitVertex(); //emit new particle
                        EndPrimitive();
                    }

                    type = 0;
                    position = Position[0];
                    velocity = Velocity[0];
                    acceleration = Acceleration[0];
                    color = Color[0];
                    size = Size[0];
                    age = _age;
                    EmitVertex(); //emit emitter :D
                    EndPrimitive();

                }
                else //regular particles
                {
                    if(_age<=Lifetime[0])
                    {
                        type = Type[0];
                        position = Position[0];
                        velocity = Velocity[0];
                        acceleration = Acceleration[0];
                        velocity+=acceleration;
                        position+=velocity;
                        color = Color[0];
                        size = Size[0];
                        age = _age;
                        EmitVertex(); //emit regular vertex wich is still alive
                        EndPrimitive();
                    }
                }
            }
            );

void pxgParticleSystem::InitShaders(std::string particleFunc)
{
    if(particleFunc.empty())
    {
        PXG::Log("pxgParticleSystem::InitShaders()  error: particleFunc is empty.", ERR);
        return;
    }

    const char* update_vs_src =
            "#version 150\n"
            SHADER(
            in vec3 particlePos;
            in vec3 particleVelocity;
            in vec3 particleAcceleration;
            in vec4 particleColor;
            in float particleSize;
            in float particleAge;
            in float particleLifetime;
            in int particleType;

            out vec3 Position;
            out vec3 Velocity;
            out vec3 Acceleration;
            out vec4 Color;
            out float Size;
            out float Age;
            out float Lifetime;
            out int Type;

            uniform mat4 MVP;

            void main()
            {
                Velocity = particleVelocity;
                Position = particlePos;
                Acceleration = particleAcceleration;
                Age = particleAge;
                Lifetime = particleLifetime;
                Size = particleSize;
                Color = particleColor;
                Type = particleType;
            }

            );

    std::string update_gs_src =
            "#version 330\n"
            "#define MAX_VERTS 30 \n"
            SHADER(

                layout ( points ) in;
                layout ( points, max_vertices = MAX_VERTS ) out;

                in vec3 Position[];
                in vec3 Velocity[];
                in vec3 Acceleration[];
                in vec4 Color[];
                in float Size[];
                in float Age[];
                in float Lifetime[];
                in int Type[];

                out vec3 position;
                out vec3 velocity;
                out vec3 acceleration;
                out vec4 color;
                out float size;
                out float lifetime;
                out float age;
                out int type;

                uniform int numVertsPerStep;
                uniform vec3 uVelocity;
                uniform vec3 uAcceleration;
                uniform vec4 uColor;
                uniform float uSize;
                uniform int uLifetime;

                )"\n"+particleFunc+"\n"
                SHADER(

                void main()
                {
                    pxgParticleUpdateFunc();
                }

                );

    updateShader = new pxgShader();

    const char* gs1 = update_gs_src.c_str();
    updateShader->VS(&update_vs_src);
    updateShader->GS(&gs1);
    std::vector<std::string> varyings;
    varyings.push_back("position");
    varyings.push_back("velocity");
    varyings.push_back("acceleration");
    varyings.push_back("color");
    varyings.push_back("age");
    varyings.push_back("size");
    varyings.push_back("type");
    std::vector<std::string> attribs;
    attribs.push_back("particlePos");
    attribs.push_back("particleVelocity");
    attribs.push_back("particleAcceleration");
    attribs.push_back("particleColor");
    attribs.push_back("particleSize");
    attribs.push_back("particleAge");
    attribs.push_back("particleType");
    updateShader->LinkTransformFeedback(PXG_VERTEX2D,varyings,attribs);

    const char* vs_render =
            "#version 150\n"
            SHADER(
                in vec3 particlePos;
                in vec3 particleVelocity;
                in vec3 particleAcceleration;
                in vec4 particleColor;
                in float particleSize;
                in float particleAge;
                in int particleType;

                out vec4 Color;

                uniform mat4 MVP;

                void main()
                {
                    gl_PointSize = particleSize;
                    gl_Position = MVP* vec4(particlePos,1);
                    Color = particleColor;
                }

                );

    const char* fs_render =
    "#version 150\n"
    SHADER(
        in vec4 Color;

        out vec4 outColor;

        uniform sampler2D diffuse;
        uniform bool useTexture;

        void main()
        {
            if(useTexture)
                outColor = texture(diffuse,gl_PointCoord);
            else
                outColor = Color;
            if(outColor.a<=0.01)
                discard;
        }
    );

    renderShader = new pxgShader();
    renderShader->VS(&vs_render);
    renderShader->FS(&fs_render);
    renderShader->Link(PXG_VERTEX2D,attribs);
    PXG::Log("pxgParticleSystem::InitShaders(): successfully initialised shaders");

}

void pxgParticleSystem::InitBuffers()
{
    if(maxParticles<=0)
    {
        PXG::Log("pxgParticleSystem::InitBuffers() error: maxParticles<=0", ERR);
        return;
    }
    PXG::glGenTransformFeedbacks(2,feedbacks);
    PXG::glGenBuffers(2,buffers);

    Particle *particles = new Particle[maxParticles];
    particles[0].lifetime = 0.0f;
    particles[0].color = vec3(1.0f);
    particles[0].type = 0; //launcher
    particles[0].pos = vec3(0.0f);
    particles[0].velocity = vec3(0,0.05f,0);
    particles[0].acceleration = vec3(0,0.001f,0);
    particles[0].size = 5.0f;
    particles[0].age = 0.0f;

    for(int i = 0;i<2;i++)
    {
        PXG::glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedbacks[i]);
        PXG::glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        PXG::glBufferData(GL_ARRAY_BUFFER,sizeof(Particle)*maxParticles, particles, GL_DYNAMIC_DRAW);
        PXG::glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffers[i]);
    }

    PXG::Log("pxgParticleSystem::InitBuffers(): successfully initialised particle buffers");

}

void pxgParticleSystem::SetTexture(pxgTexture* t)
{
    texture = t;
}

void pxgParticleSystem::SetColors(std::vector<vec4> cls)
{
    colors = cls;
}

void pxgParticleSystem::SetSize(float min, float max)
{
    minSize = min;
    maxSize = max;
}

void pxgParticleSystem::SetRotationRange(vec3 min, vec3 max)
{
    minRot = min;
    maxRot = max;
}

void pxgParticleSystem::SetStartVelocity(vec3 min, vec3 max)
{
    minStartVel = min;
    maxStartVel = max;
}

void pxgParticleSystem::SetLifetime(float min, float max)
{
    minLifetime = min;
    maxLifetime = max;
}

void pxgParticleSystem::SetSpawnRadius(float r)
{
    spawnRadius = r;
}

void pxgParticleSystem::SetAcceleration(vec3 min, vec3 max)
{
    minAcceleration = min;
    maxAcceleration = max;
}

void pxgParticleSystem::SetSpawnRate(int particlesPerFrame)
{
    spawnRate = particlesPerFrame;
}

void pxgParticleSystem::SetMaxParticles(int max)
{
    maxParticles = max;
}

void pxgParticleSystem::Update()
{

}

bool pxgParticleSystem::Render()
{
    return true;
}

void pxgParticleSystem::Destroy()
{

}
