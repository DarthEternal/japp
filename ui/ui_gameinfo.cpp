// Copyright (C) 1999-2000 Id Software, Inc.
//
//
// gameinfo.c
//

#include "ui_local.h"

// arena and bot info

int ui_numBots;
static char *ui_botInfos[MAX_BOTS];

static int ui_numArenas;
static char *ui_arenaInfos[MAX_ARENAS];

int UI_ParseInfos(char *buf, int max, char *infos[]) {
    char *token;
    int count;
    char key[MAX_TOKEN_CHARS];
    char info[MAX_INFO_STRING];

    count = 0;

    while (1) {
        token = COM_Parse((const char **)&buf);
        if (!token[0]) {
            break;
        }
        if (strcmp(token, "{")) {
            Com_Printf("Missing { in info file\n");
            break;
        }

        if (count == max) {
            Com_Printf("Max infos exceeded\n");
            break;
        }

        info[0] = '\0';
        while (1) {
            token = COM_ParseExt((const char **)&buf, qtrue);
            if (!token[0]) {
                Com_Printf("Unexpected end of info file\n");
                break;
            }
            if (!strcmp(token, "}")) {
                break;
            }
            Q_strncpyz(key, token, sizeof(key));

            token = COM_ParseExt((const char **)&buf, qfalse);
            if (!token[0]) {
                strcpy(token, "<NULL>");
            }
            Info_SetValueForKey(info, key, token);
        }
        // NOTE: extra space for arena number
        infos[count] = (char *)UI_Alloc(strlen(info) + strlen("\\num\\") + strlen(va("%d", MAX_ARENAS)) + 1);
        if (infos[count]) {
            strcpy(infos[count], info);
#ifdef _DEBUG
            if (trap->Cvar_VariableValue("com_buildScript")) {
                const char *botFile = Info_ValueForKey(info, "personality");
                if (botFile && botFile[0]) {
                    int fh = 0;
                    trap->FS_Open(botFile, &fh, FS_READ);
                    if (fh) {
                        trap->FS_Close(fh);
                    }
                }
            }
#endif
            count++;
        }
    }
    return count;
}

static void UI_LoadArenasFromFile(char *filename) {
    int len;
    fileHandle_t f;
    char buf[MAX_ARENAS_TEXT];

    len = trap->FS_Open(filename, &f, FS_READ);
    if (!f) {
        trap->Print(va(S_COLOR_RED "file not found: %s\n", filename));
        return;
    }
    if (len >= MAX_ARENAS_TEXT) {
        trap->Print(va(S_COLOR_RED "file too large: %s is %i, max allowed is %i", filename, len, MAX_ARENAS_TEXT));
        trap->FS_Close(f);
        return;
    }

    trap->FS_Read(buf, len, f);
    buf[len] = 0;
    trap->FS_Close(f);

    ui_numArenas += UI_ParseInfos(buf, MAX_ARENAS - ui_numArenas, &ui_arenaInfos[ui_numArenas]);
}

#define MAPSBUFSIZE (MAX_MAPS * 64)
void UI_LoadArenas(void) {
    int numdirs, i, n, dirlen;
    char filename[MAX_QPATH], *dirptr;
    static char dirlist[MAPSBUFSIZE];

    dirlist[0] = '\0';

    ui_numArenas = 0;
    uiInfo.mapCount = 0;

    // get all arenas from .arena files
    numdirs = trap->FS_GetFileList("scripts", ".arena", dirlist, ARRAY_LEN(dirlist));
    dirptr = dirlist;
    for (i = 0; i < numdirs; i++, dirptr += dirlen + 1) {
        dirlen = strlen(dirptr);
        strcpy(filename, "scripts/");
        strcat(filename, dirptr);
        UI_LoadArenasFromFile(filename);
    }
    //	trap->Print( va( "%i arenas parsed\n", ui_numArenas ) );
    if (UI_OutOfMemory()) {
        trap->Print(S_COLOR_YELLOW "WARNING: not anough memory in pool to load all arenas\n");
    }

    for (n = 0; n < ui_numArenas; n++) {
        uiInfo.mapList[uiInfo.mapCount].cinematic = -1;
        uiInfo.mapList[uiInfo.mapCount].mapLoadName = String_Alloc(Info_ValueForKey(ui_arenaInfos[n], "map"));
        uiInfo.mapList[uiInfo.mapCount].mapName = String_Alloc(Info_ValueForKey(ui_arenaInfos[n], "longname"));
        uiInfo.mapList[uiInfo.mapCount].levelShot = -1;
        uiInfo.mapList[uiInfo.mapCount].imageName = String_Alloc(va("levelshots/%s", uiInfo.mapList[uiInfo.mapCount].mapLoadName));
        uiInfo.mapList[uiInfo.mapCount].typeBits = BG_GetMapTypeBits(Info_ValueForKey(ui_arenaInfos[n], "type"));

        if (++uiInfo.mapCount >= MAX_MAPS) {
            trap->Print(S_COLOR_YELLOW "Warning: Too many maps in .arena files (%i > %i)", uiInfo.mapCount, MAX_MAPS);
            break;
        }
    }
}

static void UI_LoadBotsFromFile(const char *filename) {
    int len;
    fileHandle_t f;
    char buf[MAX_BOTS_TEXT];
    char *stopMark;

    len = trap->FS_Open(filename, &f, FS_READ);
    if (!f) {
        trap->Print(va(S_COLOR_RED "file not found: %s\n", filename));
        return;
    }
    if (len >= MAX_BOTS_TEXT) {
        trap->Print(va(S_COLOR_RED "file too large: %s is %i, max allowed is %i", filename, len, MAX_BOTS_TEXT));
        trap->FS_Close(f);
        return;
    }

    trap->FS_Read(buf, len, f);
    buf[len] = 0;

    stopMark = strstr(buf, "@STOPHERE");

    // This bot is in place as a mark for modview's bot viewer.
    // If we hit it just stop and trace back to the beginning of the bot define and cut the string off.
    // This is only done in the UI and not the game so that "test" bots can be added manually and still
    // not show up in the menu.
    if (stopMark) {
        int startPoint = stopMark - buf;

        while (buf[startPoint] != '{') {
            startPoint--;
        }

        buf[startPoint] = 0;
    }

    trap->FS_Close(f);

    COM_Compress(buf);

    ui_numBots += UI_ParseInfos(buf, MAX_BOTS - ui_numBots, &ui_botInfos[ui_numBots]);
}

void UI_LoadBots(void) {
    vmCvar_t botsFile;
    int numdirs;
    char filename[128];
    char dirlist[1024];
    char *dirptr;
    int i;
    int dirlen;

    ui_numBots = 0;

    trap->Cvar_Register(&botsFile, "g_botsFile", "", CVAR_INIT | CVAR_ROM);
    if (*botsFile.string) {
        UI_LoadBotsFromFile(botsFile.string);
    } else {
        UI_LoadBotsFromFile("botfiles/bots.txt");
    }

    // get all bots from .bot files
    numdirs = trap->FS_GetFileList("scripts", ".bot", dirlist, 1024);
    dirptr = dirlist;
    for (i = 0; i < numdirs; i++, dirptr += dirlen + 1) {
        dirlen = strlen(dirptr);
        strcpy(filename, "scripts/");
        strcat(filename, dirptr);
        UI_LoadBotsFromFile(filename);
    }
    //	trap->Print( va( "%i bots parsed\n", ui_numBots ) );
}

char *UI_GetBotInfoByNumber(int num) {
    if (num < 0 || num >= ui_numBots) {
        trap->Print(va(S_COLOR_RED "Invalid bot number: %i\n", num));
        return NULL;
    }
    return ui_botInfos[num];
}

const char *UI_GetBotInfoByName(const char *name) {
    int n;
    const char *value;

    for (n = 0; n < ui_numBots; n++) {
        value = Info_ValueForKey(ui_botInfos[n], "name");
        if (!Q_stricmp(value, name)) {
            return ui_botInfos[n];
        }
    }

    return NULL;
}

int UI_GetNumBots(void) { return ui_numBots; }

const char *UI_GetBotNameByNumber(int num) {
    const char *info = UI_GetBotInfoByNumber(num);
    return info ? Info_ValueForKey(info, "name") : "Kyle";
}
