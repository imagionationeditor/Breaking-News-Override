#include "ServerStats.h"
#include "ScriptMgr.h"
#include <sstream>

std::string ServerStats::GetHTMLStyle()
{
    return R"(
        <style>
            .server-stats {
                font-family: Arial, sans-serif;
                width: 100%;
                border-collapse: collapse;
                margin-bottom: 20px;
            }
            .server-stats th {
                background-color: #2c3e50;
                color: white;
                padding: 10px;
                text-align: left;
            }
            .server-stats td {
                padding: 8px;
                border-bottom: 1px solid #ddd;
            }
            .top-section {
                background-color: #f39c12;
                color: white;
                padding: 10px;
                margin-bottom: 15px;
                border-radius: 5px;
                font-weight: bold;
            }
            .news-section {
                background-color: #2ecc71;
                color: white;
                padding: 15px;
                margin-top: 20px;
                border-radius: 5px;
            }
            .news-title {
                font-size: 20px;
                font-weight: bold;
                margin-bottom: 10px;
                color: #fff;
            }
            .news-content {
                line-height: 1.6;
                color: #fff;
            }
        </style>
    )";
}

std::string ServerStats::FormatTableRow(const std::string& name, const std::string& value, const std::string& extra)
{
    std::stringstream ss;
    ss << "<div class='top-section'>";
    ss << name << " - " << value;
    if (!extra.empty())
        ss << " (" << extra << ")";
    ss << "</div>";
    return ss.str();
}

std::string ServerStats::GetTopGuilds(uint32 count)
{
    std::stringstream ss;
    ss << "<div style='margin-bottom: 10px;'><b>🏆 Top Guild:</b></div>";

    QueryResult result = CharacterDatabase.PQuery(
        "SELECT gp.guild_name, gp.points, c.name "
        "FROM guild_points gp "
        "LEFT JOIN guild g ON g.guildid = gp.guild_id "
        "LEFT JOIN characters c ON c.guid = g.leaderguid "
        "ORDER BY gp.points DESC LIMIT %u", count);

    if (result)
    {
        Field* fields = result->Fetch();
        std::string guildName = fields[0].GetString();
        uint32 points = fields[1].GetUInt32();
        std::string leaderName = fields[2].GetString();
        
        ss << FormatTableRow(guildName, std::to_string(points) + " points", "Leader: " + leaderName);
    }

    return ss.str();
}

std::string ServerStats::GetTopArenaTeams(uint32 count)
{
    std::stringstream ss;
    ss << "<div style='margin-bottom: 10px;'><b>⚔️ Top Arena Team (3v3):</b></div>";

    QueryResult result = CharacterDatabase.PQuery(
        "SELECT at.name, at.rating, c.name "
        "FROM arena_team at "
        "LEFT JOIN characters c ON c.guid = at.captainguid "
        "WHERE at.type = 3 "
        "ORDER BY at.rating DESC LIMIT %u", count);

    if (result)
    {
        Field* fields = result->Fetch();
        std::string teamName = fields[0].GetString();
        uint32 rating = fields[1].GetUInt32();
        std::string captainName = fields[2].GetString();
        
        ss << FormatTableRow(teamName, std::to_string(rating) + " rating", "Captain: " + captainName);
    }

    return ss.str();
}

std::string ServerStats::GetTopKillers(uint32 count)
{
    std::stringstream ss;
    ss << "<div style='margin-bottom: 10px;'><b>💀 Top PvP Killer:</b></div>";

    QueryResult result = CharacterDatabase.PQuery(
        "SELECT name, totalKills, level "
        "FROM characters "
        "ORDER BY totalKills DESC LIMIT %u", count);

    if (result)
    {
        Field* fields = result->Fetch();
        std::string name = fields[0].GetString();
        uint32 kills = fields[1].GetUInt32();
        uint8 level = fields[2].GetUInt8();
        
        ss << FormatTableRow(name, std::to_string(kills) + " kills", "Level " + std::to_string(level));
    }

    return ss.str();
}

std::string ServerStats::GetTopAchievements(uint32 count)
{
    std::stringstream ss;
    ss << "<div style='margin-bottom: 10px;'><b>🌟 Top Achievement:</b></div>";

    QueryResult result = CharacterDatabase.PQuery(
        "SELECT c.name, COUNT(ca.achievement) as ach_count, "
        "SUM(a.points) as total_points "
        "FROM characters c "
        "JOIN character_achievement ca ON c.guid = ca.guid "
        "JOIN achievement a ON ca.achievement = a.ID "
        "GROUP BY c.guid "
        "ORDER BY total_points DESC LIMIT %u", count);

    if (result)
    {
        Field* fields = result->Fetch();
        std::string name = fields[0].GetString();
        uint32 achCount = fields[1].GetUInt32();
        uint32 points = fields[2].GetUInt32();
        
        ss << FormatTableRow(name, std::to_string(points) + " points", 
                           std::to_string(achCount) + " achievements");
    }

    return ss.str();
}

std::string ServerStats::GetServerNews()
{
    std::stringstream ss;
    ss << "<div class='news-section'>";
    ss << "<div class='news-title'>📢 Server News</div>";
    ss << "<div class='news-content'>";

    // Read news from config file
    for (int i = 1; i <= 10; i++) // Support up to 10 news lines
    {
        std::string newsKey = "News.Line" + std::to_string(i);
        std::string newsText = sConfigMgr->GetOption<std::string>(newsKey, "");
        
        if (newsText.empty())
            break;
            
        ss << "• " << newsText << "<br>";
    }

    ss << "</div>";
    ss << "</div>";
    return ss.str();
}

std::string ServerStats::GenerateHTML()
{
    std::stringstream ss;
    
    // Add CSS styles
    ss << GetHTMLStyle();
    
    // Add content
    ss << "<div style='padding: 20px;'>";
    ss << GetTopGuilds();
    ss << GetTopArenaTeams();
    ss << GetTopKillers();
    ss << GetTopAchievements();
    ss << GetServerNews();
    ss << "</div>";
    
    return ss.str();
} 