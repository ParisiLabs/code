#pragma once

#include "inexor/server/server_enums.hpp"
#include "inexor/server/server_teams.hpp"
#include "inexor/server/server_core.hpp"

#include "inexor/enumerations/enum_admin_levels.hpp"

#include "inexor/server/server_clientinfo.hpp"
#include "inexor/server/server_gamestate.hpp"
#include "inexor/server/server_hitinfo.hpp"
#include "inexor/server/server_administration.hpp"



namespace inexor {
namespace server {

    /// 
    static const int DEATHMILLIS = 300;

    /// 
    extern int gamemode, gamemillis, gamelimit, nextexceeded, gamespeed, interm;
    extern bool notgotitems, shouldstep, gamepaused, teamspersisted;
    extern hashset<teaminfo> teaminfos;

    /// this structure will be declared below
    struct clientinfo;

    /// TODO: not sure if this is correct!
    extern vector<clientinfo *> connects, clients, bots;

    /// 
    void changegamespeed(int val, clientinfo *ci = NULL);

    /// 
    void checkintermission();

    /// 
    void startintermission();

    // Checks if the game has ended because only one player is still alive.
    // It does this by checking if less than 2 players have their state set to alive.
    // This means, the game will also end if someone is gagging
    // If only one is still alive this method forces intermission.
    void checklms();

    /// 
    void suicide(clientinfo *ci);

    /// 
    clientinfo *getinfo(int n);

    /// 
    void dodamage(clientinfo *target, clientinfo *actor, int damage, int gun, const vec &hitpush = vec(0, 0, 0));

    int numclients(int exclude = -1, bool nospec = true, bool noai = true, bool priv = false);

};
};
