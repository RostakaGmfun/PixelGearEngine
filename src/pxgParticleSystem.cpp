/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgParticleSystem.h>
#include <pxgTools.h>
#include <pxgShader.h>
#include <pxgScene.h>

#define SHADER(s) #s

pxgParticleSystem::pxgParticleSystem(): pxgNode(), updateShader(NULL), renderShader(NULL), scene(NULL), texture(NULL)
{

}

std::string pxgParticleSystem::pxgDefaultUpdateFunc =
        SHADER(
            void pxgParticleUpdateFunc()
            {
                Velocity = particleVelocity+particleAcceleration;
                Position = particlePos+Velocity;
                Acceleration = particleAcceleration;
                Age = particleAge+1;
                Size = particleSize;
                Color = particleColor;
            }
            );

void pxgParticleSystem::InitShader(std::string particleFunc)
{
    if(particleFunc.empty())
    {
        PXG::Log("pxgParticleSystem::InitShader()  error: particleFunc is empty.", ERR);
        return;
    }

    updateShader = new pxgShader();
    std::string update_vs_src =
           std::string(
            "#version 130\n"
            SHADER(
            in vec3 particlePos;
            in vec3 particleVelocity;
            in vec3 particleAcceleration;
            in vec4 particleColor;
            in float particleSize;
            in float particleAge;

            out vec3 Position;
            out vec3 Velocity;
            out vec3 Acceleration;
            out vec4 Color;
            out float Size;
            out float Age;

            uniform float Time;
            uniform float particleLifetime;

            uniform mat4 MVP;
            ))+"\n"+particleFunc+"\n"
            SHADER(

            void main()
            {
                pxgParticleUpdateFunc();
            }

            );

    const char* vs1 = update_vs_src.c_str();
    updateShader->VS(&vs1);
    std::vector<std::string> varyings;
    varyings.push_back("Position");
    varyings.push_back("Velocity");
    varyings.push_back("Acceleration");
    varyings.push_back("Color");
    varyings.push_back("Age");
    varyings.push_back("Size");
    std::vector<std::string> attribs;
    attribs.push_back("particlePos");
    attribs.push_back("particleVelocity");
    attribs.push_back("particleAcceleration");
    attribs.push_back("particleColor");
    attribs.push_back("particleSize");
    attribs.push_back("particleAge");
    updateShader->LinkTransformFeedback(PXG_VERTEX2D,varyings,attribs);

    const char* vs_render =
            "#version 130\n"
            SHADER(
                in vec3 particlePos;
                in vec3 particleVelocity;
                in vec3 particleAcceleration;
                in vec4 particleColor;
                in float particleSize;
                in float particleAge;

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
    "#version 130\n"
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

}

void pxgParticleSystem::InitBuffers()
{
    PXG::glGenTransformFeedbacks(2,feedbacks);
    PXG::glGenBuffers(2,buffers);

    int maxParticles =
    Particle particles[100];

    for(int i = 0;i< 2;i++)
    {
        PXG::glBindTransfromFeedback(GL_TRANSFORM_FEEDBACK, feedbacks[i]);
        PXG::glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
        PXG::glBufferData(GL_ARRAY_BUFFER,sizeof(particles), particles, GL_DYNAMIC_DRAW);
        PXG::glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffers[0]);
    }

}

void pxgParticleSystem::Update()
{

}

bool pxgParticleSystem::Render()
{

}

void pxgParticleSystem::Destroy()
{

}
