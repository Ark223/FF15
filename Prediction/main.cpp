
#include "API/api_public.h"
#include "PrivateAPI.h"
#include "Program.h"

namespace IPrediction
{
    static Program* program = nullptr;

    static uintptr_t __stdcall LoadAPI()
    {
        return (uintptr_t)new IPredictionAPI();
    }

    static bool Init(NS SurrenderAt15* api)
    {
        program = new Program(api);
        if (!program->DependencyCheck()) return false;

        auto communication = new PluginCommunication();
        communication->get_api_vtable = LoadAPI;
        communication->plugin_name = "Prediction";
        communication->version = "1";
        api->insert_api(communication);

        return true;
    }

    static bool Unload()
    {    
        delete program;
        program = nullptr;
        return true;
    }
}

extern "C" __declspec(dllexport) plugin_t PLUGIN =
{
    "2025.03.29.02",
    "Prediction",
    IPrediction::Init,
    IPrediction::Unload
};

BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD dwReason, const LPVOID lpReserved)
{
    return true;
}
