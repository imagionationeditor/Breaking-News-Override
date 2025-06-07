#ifndef MOD_BREAKING_NEWS_H
#define MOD_BREAKING_NEWS_H

#include "ScriptMgr.h"
#include "Config.h"
#include "Player.h"
#include "World.h"
#include "Warden.h"
#include "WardenWin.h"
#include "fmt/format.h"

#include <algorithm>
#include <fstream>
#include <iostream>

// Forward declarations
class Warden;
class WardenPayloadMgr;

// Extern variables declarations
extern bool bn_Enabled;
extern std::string bn_Title;
extern std::string bn_Body;
extern std::string bn_Formatted;

// Constants
const std::string _prePayload = "wlbuf = '';";
const std::string _postPayload = "local a,b=loadstring(wlbuf)if not a then message(b)else a()end";
const std::string _midPayloadFmt = "local a=ServerAlertFrame;local b=ServerAlertText;local c=ServerAlertTitle;local d=CharacterSelect;if a~=nil or b~=nil or c~=nil or d~=nil then a:SetParent(d)ServerAlertTitle:SetText('{}')ServerAlertText:SetText('{}')a:Show()else message('ServerAlert(Frame|Text|Title)/CharacterSelect Frame is nil.')end";
const uint16 _prePayloadId = 9500;
const uint16 _postPayloadId = 9501;
const uint16 _tmpPayloadId = 9502;

// Function declarations
std::vector<std::string> GetChunks(std::string s, uint8_t chunkSize);
void SendChunkedPayload(Warden* warden, std::string payload, uint32 chunkSize);

class BreakingNewsServerScript : public ServerScript
{
public:
    BreakingNewsServerScript() : ServerScript("BreakingNewsServerScript") { }

    bool CanPacketSend(WorldSession* session, WorldPacket& packet) override;

private:
    std::vector<std::string> GetChunks(std::string s, uint8_t chunkSize);
    void SendChunkedPayload(Warden* warden, WardenPayloadMgr* payloadMgr, std::string payload, uint32 chunkSize);
};

class BreakingNewsWorldScript : public WorldScript
{
public:
    BreakingNewsWorldScript() : WorldScript("BreakingNewsWorldScript") { }

    void OnAfterConfigLoad(bool reload) override;
};

void AddBreakingNewsScripts();

#endif // MOD_BREAKING_NEWS_H
