#include <iostream>

#include "Util/Logger.h"
#include "Util/Game.h"
#include "Util/_UUID.h"
#include "Util/Terminal.h"

auto main() -> int {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    DestroyWindow(GetConsoleWindow());
    Terminal::setup();
    Game::close();

    system("start https://discord.gg/kodiakext");
    MessageBoxA(nullptr, "Join discord.gg/kodiakext", "Kodiak", MB_OK | MB_ICONINFORMATION);

    Game::removeSpecificFiles("\\LocalState", {".ent", ".cache"});
    Game::remove("\\LocalState\\logs");
    Game::remove("\\LocalState\\premium_cache");
    Game::remove("\\LocalState\\crash");
    Game::remove("\\LocalState\\cdn");
    Game::remove("\\LocalState\\speech");
    Game::remove("\\LocalState\\treatments");
    Game::remove("\\LocalState\\bootstrap_settings.json");
    Game::remove("\\LocalState\\games\\com.mojang\\minecraftpe\\hs");
    Game::remove("\\LocalState\\games\\com.mojang\\minecraftpe\\storage_object");
    Game::remove("\\LocalState\\games\\com.mojang\\minecraftpe\\telemetry_info.json");
    Game::remove("\\LocalState\\games\\com.mojang\\minecraftpe\\clientId.txt");
    Game::remove("\\LocalState\\games\\com.mojang\\minecraftpe\\catalog_info.json");
    Game::remove("\\LocalState\\games\\com.mojang\\minecraftpe\\NonAssertErrorLog.txt");
    Game::remove("\\LocalState\\games\\com.mojang\\minecraftpe\\NotificationCenterSettings.dat");
    Game::remove("\\LocalState\\games\\com.mojang\\minecraftpe\\splitscreen_appearance.json");
    Game::remove("\\LocalState\\games\\com.mojang\\minecraftpe\\pack_settings");
    Game::removeSpecificFiles("\\LocalState\\games\\com.mojang", {".dat"});
    Game::remove("\\AC\\Microsoft\\CryptnetUrlCache\\Content");
    Game::remove("\\AC\\Microsoft\\CryptnetUrlCache\\MetaData");
    Game::remove("\\AC\\Microsoft\\CryptnetUrlCache");
    Game::remove("\\AC\\Microsoft");
    Game::remove("\\AC\\Temp");
    Game::remove("\\AC\\TokenBroker\\Cache");
    Game::remove("\\AC\\TokenBroker");
    Game::remove("\\AC");
    Game::remove("\\AppData");
    Game::remove("\\Settings");
    Game::remove("\\SystemAppData");
    Game::remove("\\TempState");
    Game::remove("\\LocalCache\\lastdevicesessionid.txt");
    Game::remove("\\LocalCache\\minecraftpe\\ContentCache\\PatchNotes");
    Game::remove("\\LocalCache\\minecraftpe\\ContentCache\\ThirdPartyServer");
    Game::remove("\\LocalCache\\minecraftpe\\ContentCache");
    Game::remove("\\LocalCache\\minecraftpe\\ClubsScreenshots");
    Game::remove("\\LocalCache\\minecraftpe\\CatalogScratch");
    Game::remove("\\LocalCache\\minecraftpe\\CatalogCache");
    Game::remove("\\LocalCache\\minecraftpe\\AchievementIcons");
    Game::remove("\\LocalCache\\minecraftpe\\FriendIcons");
    Game::remove("\\LocalCache\\minecraftpe\\images");
    Game::remove("\\LocalCache\\minecraftpe\\font");
    Game::remove("\\LocalCache\\minecraftpe\\ServerContent");
    Game::remove("\\LocalCache\\minecraftpe\\persona");
    Game::remove("\\LocalCache\\minecraftpe\\Treatments");
    Game::remove("\\LocalCache\\minecraftpe\\TempImportPacks");
    Game::remove("\\LocalCache\\minecraftpe\\Progressions");
    Game::remove("\\LocalCache\\minecraftpe\\DownloadTemp");
    Game::remove("\\LocalCache\\minecraftpe\\ShaderCache");
    Game::remove("\\LocalCache\\minecraftpe\\Flighting");
    Game::remove("\\LocalCache\\minecraftpe\\MCProfileCache");
    Game::remove("\\LocalCache\\minecraftpe\\MessagingService");
    Game::remove("\\LocalCache\\minecraftpe\\XblProfileCache");

    system("start minecraft://");

    MessageBoxA(nullptr, "Just click on 'Ok' when the game is launched and you're in the menu (please wait until you're connected to xbox live).", "Kodiak", MB_OK | MB_ICONWARNING);
    while (Game::handle == nullptr || Game::module == nullptr)
    {
        Game::handle = Game::GetProcessByName("Minecraft.Windows.exe");
        Game::module = Game::GetModule(Game::handle);
    }

    Game::baseAddress = reinterpret_cast<uintptr_t>(Game::module);

    std::string deviceId = Game::getDeviceId();
    std::string newDID = _UUID::v4();

    std::vector<void*> matches = Game::scanString(Game::handle, deviceId);
    for (void* addr : matches) {
        Game::patchBytes(addr, (void *) newDID.c_str(), newDID.size());
    }

    MessageBoxA(nullptr, "Successful Spoof!", "Kodiak", MB_OK | MB_ICONINFORMATION);
    return 0;
}