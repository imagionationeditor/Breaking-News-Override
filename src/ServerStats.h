#ifndef _SERVER_STATS_H
#define _SERVER_STATS_H

#include "Player.h"
#include "Guild.h"
#include "ArenaTeam.h"
#include "DatabaseEnv.h"
#include <string>

class ServerStats
{
public:
    static std::string GenerateHTML();
    
private:
    static std::string GetTopGuilds(uint32 count = 1);
    static std::string GetTopArenaTeams(uint32 count = 1);
    static std::string GetTopKillers(uint32 count = 1);
    static std::string GetTopAchievements(uint32 count = 1);
    static std::string GetServerNews();
    
    static std::string FormatTableRow(const std::string& name, const std::string& value, const std::string& extra = "");
    static std::string GetHTMLStyle();
};

#endif 