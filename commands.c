/*
==============================
 Game commands
==============================
*/

#include "eth.h"

void sysInfoCommand() {
	if (syscall_UI_Argc() != 1) {
		orig_syscall(UI_PRINT, "^nSpam with system infos\n");
		orig_syscall(UI_PRINT, "^nUsage: eth_sysinfo\n");
		return;
	}
	
	char *kernel = strdup(getOutputSystemCommand("uname -sr"));
	char *proc = strdup(getOutputSystemCommand("grep 'model name' /proc/cpuinfo | cut -d' ' -f3-"));
	
	char buf[512];
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "say \"^nkernel^0[^3%s^0]\"\n", kernel);
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, buf);

	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "say \"^nproc^0[^3%s^0]\"\n", proc);
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, buf);

	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "say \"^ngfx^0[^3%s^0] [^3%s^0]\"\n", eth.cgs_glconfig.vendor_string, eth.cgs_glconfig.renderer_string);
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, buf);

	free(kernel);
	free(proc);
}

// Stupid and useless spam ;) from http://www.network-science.de/ascii/
void spamCommand() {
	if (syscall_UI_Argc() != 1) {
		orig_syscall(UI_PRINT, "^nSpam with 'eth powaa'\n");
		orig_syscall(UI_PRINT, "^nUsage: eth_spam\n");
		return;
	}
	
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "say \"^1      _   _\"\n");
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "say \"^1  ___| |_| |__    _ __   _____      ____ _  __ _\"\n");
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "say \"^1 / _ \\ __| '_ \\  | '_ \\ / _ \\ \\ /\\ / / _` |/ _` |\"\n");
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "say \"^1|  __/ |_| | | | | |_) | (_) \\ V  V / (_| | (_| |\"\n");
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "say \"^1 \\___|\\__|_| |_| | .__/ \\___/ \\_/\\_/ \\__,_|\\__,_|\"\n");
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "say \"^1                 |_|\"\n");
}

void specsCommand() {
    if (syscall_UI_Argc() != 1) {
        orig_syscall(UI_PRINT, "^nSpam with 'eth specs'\n");
        orig_syscall(UI_PRINT, "^nUsage: eth_specs\n");
        return;
    }
    char cmd[512];
    memset(cmd, 0, sizeof(cmd));
    int count = 0;
    for (; count < seth.specCount; count++) {
        strcat(cmd, seth.specNames[count]);
        strcat(cmd, ", ");
    }
    orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, va("say ^2 Spectators ^7 viewing me^9: %s\n", cmd));
    
}

void botspamCommand() {
	if (syscall_UI_Argc() != 1) {
		orig_syscall(UI_PRINT, "^nSpam with 'botspam'\n");
		orig_syscall(UI_PRINT, "^nUsage: eth_botspam\n");
		return;
	}


	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "say \"^3[^OETH 1.1-1 Rabbmod ^3] ^3By Rabbnix.com\"\n");


}

void execCommand() {
	if (syscall_UI_Argc() == 1)
		orig_syscall(UI_PRINT, "^nExecute a system command\n");

	if ((syscall_UI_Argc() == 1) || (syscall_UI_Argc() > 2)) {
		orig_syscall(UI_PRINT, "^nUsage: eth_exec [COMMAND]\n");
		return;
	}
	
	if (syscall_UI_Argc() == 2) {
		char buf[128];
		snprintf(buf, sizeof(buf), "echo \"%s\"\n", getOutputSystemCommand(syscall_UI_Argv(1)));
		orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, buf);
	}
}

void findETHCommand (void) {
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "vsay " ETH_REQUEST_STR "\n");
}

void cleanPK3Command (void) {
	if (syscall_UI_Argc() != 1) {
		orig_syscall(UI_PRINT, "^nRemove all small pk3\n");
		orig_syscall(UI_PRINT, "^nUsage: " ETH_CMD_PREFIX "cleanpk3\n");
		return;
	}

	system("find ~/.etwolf/ -name '*.pk3' -size -1200k -delete");
	orig_syscall(UI_PRINT, "^nClean pk3 done.\nYou must restart the game now.\n");
}

// For terrorising :)
void randomKickCommand (void) {
	int kickID[MAX_CLIENTS];
	int kickCount = 0;

	// Get all kickable players
	int entityNum;
	for (entityNum = 0; entityNum < MAX_CLIENTS; entityNum++) {
		if ((eth.clientInfo[entityNum].infoValid) && (eth.clientInfo[eth.cg_clientNum].team != TEAM_SPECTATOR) && (entityNum != eth.cg_clientNum) && (eth.clientInfo[entityNum].targetType != PLIST_FRIEND)) {
			if ((eth.clientInfo[entityNum].team == eth.clientInfo[eth.cg_clientNum].team) || (eth.clientInfo[entityNum].team == TEAM_SPECTATOR)) {
				kickID[kickCount] = entityNum;
				kickCount++;
			}
		}
	}

	#ifdef ETH_DEBUG
		ethDebug("randomkick: Players found for kick");
		int count = 0;
		for (; count < kickCount; count++)
			ethDebug("randomkick: id %i name %s", kickID[count], eth.clientInfo[kickID[count]].name);
	#endif

	if (kickCount) {
		char str[24];
		int randomID = (int)((float)kickCount * rand() / (RAND_MAX + 1.0));
		snprintf(str, sizeof(str), "callvote kick %i\n", kickID[randomID]);
		orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, str);
		#ifdef ETH_DEBUG
			ethDebug("randomkick: vote kicking id %i name %s", kickID[randomID], eth.clientInfo[kickID[randomID]].name);
		#endif
	} else {
		orig_syscall(UI_PRINT, "^nCouldn't find a player to votekick.\n");	
	}
}

void inviteCommand (void) {
	if (eth.clientInfo[eth.cg_clientNum].team != TEAM_SPECTATOR) {
		int i;
		char str[35];
		for (i=0; i < MAX_CLIENTS; i++) {
			if ((eth.clientInfo[i].infoValid) && (eth.clientInfo[i].targetType == PLIST_FRIEND) && (eth.clientInfo[i].team == eth.clientInfo[eth.cg_clientNum].team)) {
				if (eth.mod->type == MOD_ETPRO)
					snprintf(str, sizeof(str), "fireteam invite %d\n", i);
				else
					snprintf(str, sizeof(str), "fireteam invite %d\n", i+1); // other mods use player's id+1 for working with fireteam
				orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, str);
			}
		}
	}
}

void saveCommand (void) {
	if (syscall_UI_Argc() == 1)
		orig_syscall(UI_PRINT, "^nSave config to a file\n");

	if ((syscall_UI_Argc() == 1) || (syscall_UI_Argc() > 2)) {
		orig_syscall(UI_PRINT, "^nUsage: eth_save config_name\n");
		return;
	}
	
	if (syscall_UI_Argc() == 2) {
		char path[128 + 16];
		memset(path, 0, sizeof(path));
		snprintf(path, sizeof(path), "%s/%s", getenv("HOME"), syscall_UI_Argv(1));

		FILE *file;
		
		if ((file = fopen(path, "w")) == NULL) {
			orig_syscall(UI_PRINT, "^nCan't write config file.\n");
			ethLog("eth: can't write config file.");
			return;
		}
		
		int count = 0;
		for (; count < VARS_TOTAL; count++) {
			if (seth.value[count] == (float)(int)seth.value[count])
				fprintf(file, "%s=%i\n", seth.vars[count].cvarName, (int)seth.value[count]);
			else
				fprintf(file, "%s=%.2f\n", seth.vars[count].cvarName, seth.value[count]);
		}

		char done[512];
		snprintf(done, sizeof(done), "^nConfig '%s' saved.\n", path);

		orig_syscall(UI_PRINT, done);
	
		fclose(file);
	}
}

void loadCommand (void) {
	if (syscall_UI_Argc() == 1)
		orig_syscall(UI_PRINT, "^nLoad settings from a config file\n");

	if ((syscall_UI_Argc() == 1) || (syscall_UI_Argc() > 2)) {
		orig_syscall(UI_PRINT, "^nUsage: eth_load config_name\n");
		return;
	}
	
	if (syscall_UI_Argc() == 2) {
		FILE *file;
	
		char path[128 + 16];
		memset(path, 0, sizeof(path));
		snprintf(path, sizeof(path), "%s/%s", getenv("HOME"), syscall_UI_Argv(1));
	
		if ((file = fopen(path, "rb")) == NULL) {
			orig_syscall(UI_PRINT, "^nCan't read config file.\n");
			return;
		}
	
		// Get config file line by line
		char line[32];
		while (fgets(line, sizeof(line) - 1, file) != 0) {
			char *sep = strrchr(line, '=');
			*sep = '\0';	// Separate name from value
			// Search this var
			int count = 0;
			for (; count < VARS_TOTAL; count++) {
				if (strcmp(line, seth.vars[count].cvarName) == 0) {
					seth.value[count] = atof(sep + 1);
					break;
				} else if ((count + 1) == VARS_TOTAL) {
					ethLog("readConfig: don't know this var: [%s]", line);
				}
			}
		}

		char done[512];
		snprintf(done, sizeof(done), "^nConfig '%s' loaded.\n", path);

		orig_syscall(UI_PRINT, done);
	
		fclose(file);
	}
}

// etpro guid game command
void etproGuidCommand() {
	if (syscall_UI_Argc() == 1)
		orig_syscall(UI_PRINT, "^nChange your etpro GUID\n");

	if ((syscall_UI_Argc() == 1) || (syscall_UI_Argc() > 2)) {
		orig_syscall(UI_PRINT, "^nUsage: " ETH_CMD_PREFIX "etpro_guid [GUID]\n");
		return;
	}
	
	if (syscall_UI_Argc() == 2) {
		etproGuid = realloc(etproGuid, ETPRO_GUID_SIZE);
		memset(etproGuid, 0, ETPRO_GUID_SIZE);
		strncpy(etproGuid, syscall_UI_Argv(1), ETPRO_GUID_SIZE);
	}
	
	if (etproGuid) {
		orig_etproAntiCheat(acA, acB, acC, etproOSChecksum, acE, etproGuid);
		char txt[64];
		snprintf(txt, sizeof(txt), "^nNew etpro GUID: [^*%s^n]\n", (char *)etproGuid);
		orig_syscall(UI_PRINT, txt);
	} else {
		orig_syscall(UI_PRINT, "^nEtpro GUID is not set\n");
	}
}

// etpro OS game command
void etproOSCommand() {
	if (syscall_UI_Argc() == 1)
		orig_syscall(UI_PRINT, "^nChange your etpro OS\n");

	if ((syscall_UI_Argc() == 1) || (syscall_UI_Argc() > 2)) {
		orig_syscall(UI_PRINT, "^nUsage: " ETH_CMD_PREFIX "etpro_os [linux/win32]\n");
		return;
	}

	if (!strcasecmp(syscall_UI_Argv(1), "linux")) {
		etproOSChecksum = sethET->etproLinuxChecksum;
		orig_syscall(UI_PRINT, "^nNew etpro OS: [^*linux^n]\n");
	} else if (!strcasecmp(syscall_UI_Argv(1), "win32")) {
		etproOSChecksum = sethET->etproWin32Checksum;
		orig_syscall(UI_PRINT, "^nNew etpro OS: [^*win32^n]\n");
	} else {
		orig_syscall(UI_PRINT, "^nUnknown OS\n");
		orig_syscall(UI_PRINT, "^nUsage: " ETH_CMD_PREFIX "etpro_os [linux/win32]\n");
		return;
	}
		
	orig_etproAntiCheat(acA, acB, acC, etproOSChecksum, acE, etproGuid);
}

void amarokCommand() {
	if (syscall_UI_Argc() != 1) {
		orig_syscall(UI_PRINT, "^nSpam with amaroK infos\n");
		orig_syscall(UI_PRINT, "^nUsage: " ETH_CMD_PREFIX "amarok\n");
		return;
	}

	char *artist = strdup(getOutputSystemCommand("dcop amarok player artist"));
	char *title = strdup(getOutputSystemCommand("dcop amarok player title"));
	char *currentTime = strdup(getOutputSystemCommand("dcop amarok player currentTime"));
	char *totalTime = strdup(getOutputSystemCommand("dcop amarok player totalTime"));

	if (!strlen(artist) && !strlen(title)) {
		orig_syscall(UI_PRINT, "^namaroK: nothing to display\n");
		return;
	}

	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND,
    		va("say \"^3Playing: ^0[^n%s - %s^0]^*-^0[^n%s/%s^0] "" \n", artist, title, currentTime, totalTime));

	free(artist);
	free(title);
	free(currentTime);
	free(totalTime);
}

// Get infos through xmmsctrl - http://user.it.uu.se/~adavid/utils/
void xmmsCommand() {
    if (syscall_UI_Argc() != 1) {
        orig_syscall(UI_PRINT, "^nSpam with xmms infos\n");
        orig_syscall(UI_PRINT, "^nUsage: " ETH_CMD_PREFIX "xmms\n");
        return;
    }

	char *title = strdup(getOutputSystemCommand("xmmsctrl print %T"));
	char *currentTime = strdup(getOutputSystemCommand("xmmsctrl print %m"));
	char *totalTime = strdup(getOutputSystemCommand("xmmsctrl print %M"));

	if (!strlen(title)) {
		orig_syscall(UI_PRINT, "^nxmms: nothing to display\n");
		return;
	}

	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND,
    		va("say \"^3Playing: ^0[^n%s^0]^*-^0[^n%s/%s^0] "" \n", title, currentTime, totalTime));

	free(title);
	free(currentTime);
	free(totalTime);
}

void undercover() {
    if (syscall_UI_Argc() != 1) {
        orig_syscall(UI_PRINT, "^nSet random-name'\n");
        orig_syscall(UI_PRINT, "^nUsage: eth_undercover\n");
        return;
    }


    orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "name 'print ((0..9,a..f)[rand(16)]) for 1..40'`\n");
}

void unbanCommand() {
    if (syscall_UI_Argc() != 1) {
        orig_syscall(UI_PRINT, "^nNew pbguid and cl_guid\n");
            orig_syscall(UI_PRINT, "^nUsage: " ETH_CMD_PREFIX "unban\n");
        return;
    }
    system("mv ~/.etwolf/etmain/etkey ~/.etwolf/etmain/etkey.old");
    orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "pb_cdkeyreg \n");
    orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "CL_GUID `perl -e 'print ((0..9,a..f)[rand(16)]) for 1..40'` \n");
    orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "wait 250; name qopuifjkldfl; reconnect\n");
}

void noname() {
    if (syscall_UI_Argc() != 1) {
        orig_syscall(UI_PRINT, "^nSremove player-name'\n");
        orig_syscall(UI_PRINT, "^nUsage: eth_noname\n");
        return;
    }


    orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "name ^1   ^1\"\n");
}

void fortune() {
	char *msg;
	char buf[256];
	msg = strdup(getOutputSystemCommand("fortune -a -n 140 -s"));
	sprintf(buf,"say \"%s\"",Q_CleanStr(msg));
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, buf);
	free(msg);
}

void goHome() {
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "connect et.nixcoders.org:27962\n");
}



void registerGameCommands() {
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "exec", &execCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "sysinfo", &sysInfoCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "spam", &spamCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "find", &findETHCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "cleanpk3", &cleanPK3Command);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "randomkick", &randomKickCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "invite", &inviteCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "save", &saveCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "load", &loadCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "etpro_guid", &etproGuidCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "etpro_os", &etproOSCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "amarok", &amarokCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "xmms", &xmmsCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "botspam", &botspamCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "noname", &noname);	
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "undercover", &undercover);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "specs", &specsCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "fortune", &fortune);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "home", &goHome);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "unban", &unbanCommand);

	registerIrcCommands();
}
