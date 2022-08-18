#include "script.h"

#include "NativeMemory.hpp"
#include "Util/Logger.hpp"
#include "Util/Paths.h"
#include "Util/Versions.h"

#include <GTAVMenuBase/menukeyboard.h>
#include <inc/main.h>

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved) {
    std::string logFile = Paths::GetModuleFolder(hInstance) + modDir +
        "\\" + Paths::GetModuleNameWithoutExtension(hInstance) + ".log";
    logger.SetFile(logFile);
    Paths::SetOurModuleHandle(hInstance);

    switch (reason) {
    case DLL_PROCESS_ATTACH:
        logger.Clear();
        logger.Write(INFO, "Addon Spawner %s (built %s %s)", DISPLAY_VERSION, __DATE__, __TIME__);
        logger.Write(INFO, "Game version " + eGameVersionToString(getGameVersion()));
        scriptRegister(hInstance, ScriptMain);
        keyboardHandlerRegister(NativeMenu::OnKeyboardMessage);
        setupHooks();
        logger.Write(INFO, "Script registered");
        break;
    case DLL_PROCESS_DETACH:
        keyboardHandlerUnregister(NativeMenu::OnKeyboardMessage);
        scriptUnregister(hInstance);
        break;
    }
    return TRUE;
}
