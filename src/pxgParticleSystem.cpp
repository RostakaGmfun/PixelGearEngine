/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgParticleSystem.h>
#include <pxgTools.h>
#include <pxgShader.h>
#include <pxgShaderLib.h>

pxgParticleSystem::pxgParticleSystem(): pxgNode(), bufferA(0),
    bufferB(0), updateShader(NULL), renderShader(NULL)
{

}

std::string pxgParticleSystem::pxgDefaultUpdateFunc =
        SHADER(
            void pxgParticleUpdateFunc()
            {
                Velocity = particleVelocity+particleAcceleration;
                Poisition = particlePos+Velocity;
                Acceleration = particleAcceleration;
                Age = particleAge+1;
                Size = particleSize;
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
            in float particleSize;
            in float particleAge;

            out vec3 Position;
            out vec3 Velocity;
            out vec3 Acceleration;
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

    std::string update_fs_src =
            "#version 130\n"
            SHADER(
                void main()
                {
                    discard;
                }
                );

    const char* vs1 = update_vs_src.c_str();
    const char* fs1 = update_fs_src.c_str();
    updateShader->VS(&vs1);
    //updateShader->FS(&fs1);
    std::vector<std::string> attribs;
    attribs.push_back("particlePos");
    attribs.push_back("particleVelocity");
    attribs.push_back("particleAcceleration");
    attribs.push_back("particleAge");
    attribs.push_back("particleSize");
    updateShader->LinkTransformFeedback(PXG_VERTEX2D,attribs);
}
