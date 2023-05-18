/*
-------------------------------------------------------------------------------
    Copyright (c) Charles Carley.

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include <cstdio>
#include <vector>

#include "Image/skImage.h"
#include "RenderSystem/Cpu/rtCpuRenderSystem.h"
#include "RenderSystem/Cuda/rtCudaRenderSystem.h"
#include "RenderSystem/Loader/Ascii/rtAsciiLoader.h"
#include "RenderSystem/Loader/Blend/rtBlendLoader.h"
#include "RenderSystem/Viewer/rtViewerImpl.h"
#include "RenderSystem/rtImageTarget.h"
#include "RenderSystem/rtRenderSystem.h"
#include "Utils/CommandLine/skCommandLineParser.h"
#include "Utils/skLogger.h"

using skCmd              = skCommandLine::Parser;
using rtScenes           = rtLoader::SceneArray;
constexpr SKint32 Width  = 800;
constexpr SKint32 Height = 600;

enum RayTracerAppIds
{
    ID_BACKEND,
    ID_OUTPUT,
    ID_MAX,
};

const skCommandLine::Switch Switches[ID_MAX] = {
    {
        ID_BACKEND,
        'r',
        "backend",
        "Specify the backend renderer.\n"
        " - Where the value is one of the following values:\n"
        "   - CPU Backend: 0\n"
        "   - GPU Backend: 1\n",
        true,
        1,
    },
    {
        ID_OUTPUT,
        'o',
        "output",
        "Specify an output file.\n"
        " - Where the value is a file-system pathname.\n",
        true,
        1,
    },
};

class Application : public rtViewerImpl
{
private:
    rtLoader*      m_loader;
    int            m_backend;
    skString       m_output;
    rtImageTarget* m_image;

public:
    Application() :
        rtViewerImpl(Width, Height),
        m_loader(nullptr),
        m_backend(0),
        m_image(nullptr)
    {
        skImage::initialize();
    }

    ~Application() override
    {
        delete m_image;
        delete m_system;
        delete m_loader;
        skImage::finalize();
    }

    int parse(int argc, char** argv)
    {
        skCmd psr;
        if (psr.parse(argc, argv, Switches, ID_MAX) < 0)
            return 1;

        const skStringArray& arr = psr.getArgList();
        if (arr.empty())
        {
            skLogd(LD_ERROR, "No input file.\n");
            return 1;
        }

        m_output = psr.getValueString(ID_OUTPUT, 0);

        m_backend = psr.getValueInt(ID_BACKEND, 0, 0);
        if (m_backend != 0 && m_backend != 1)
        {
            skLogd(LD_ERROR, "Invalid back end render system.\n");
            return 1;
        }

        // Set the allocator type...
        rtAllocator::setBackend(m_backend);

        if (arr[0].endsWith(".blend"))
            m_loader = new rtBlendLoader();
        else if (arr[0].endsWith(".bascii"))
            m_loader = new rtAsciiLoader();
        else
        {
            skLogd(LD_ERROR, "Unknown file extension.\n");
            return 1;
        }

        return m_loader->load(arr[0].c_str());
    }

    int go()
    {
        if (!m_loader)
            return 1;

        rtScenes& scenes = m_loader->getScenes();

        if (scenes.empty())
        {
            printf("No scenes found\n");
            return 1;
        }
        m_scene = scenes.at(0);

        rtCameras& cameras = m_scene->getCameras();
        if (cameras.empty())
        {
            printf("No cameras found\n");
            return 1;
        }
        m_camera = cameras.at(0);

        switch (m_backend)
        {
#ifdef USING_CUDA
        case 1:
            m_system = new rtCudaRenderSystem();
            break;
#endif
        default:
            m_system = new rtCpuRenderSystem();
            break;
        }

        if (!m_output.empty())
        {
            m_image = new rtImageTarget(Width, Height);
            m_system->setTarget(m_image);
        }
        else
            m_system->setTarget(this);

        m_isInteractiveCamera = m_camera->getType() == RT_AO_USER_CAMERA;
        if (m_image)
        {
            m_scene->setFlags(m_scene->getFlags() | RM_AA);
            m_system->setMode(m_scene->getFlags());
            m_system->render(m_scene);
            m_image->save(m_output.c_str());
        }
        else
        {
            m_system->setMode(m_scene->getFlags());
            run();
        }

        return 0;
    }
};

int main(int argc, char** argv)
{

    int      status = 0;
    skLogger log;
    log.setFlags(LF_STDOUT);

    try
    {
        Application app;
        status = app.parse(argc, argv);
        if (status == 0)
            status = app.go();
    }
    catch (std::exception &e)
    {
        skLogf(LD_ERROR, "An exception occurred.\n%s\n", e.what());
    }
    catch (...)
    {
        log.logMessage(LD_ERROR, "An unhanded exception occurred.\n");
    }
    return status;
}
