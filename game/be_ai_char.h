// Copyright (C) 1999-2000 Id Software, Inc.
//

// loads a bot character from a file
int BotLoadCharacter(char *charfile, float skill);
// frees a bot character
void BotFreeCharacter(int character);
// returns a float characteristic
float Characteristic_Float(int character, int index);
// returns a bounded float characteristic
float Characteristic_BFloat(int character, int index, float min, float max);
// returns an integer characteristic
int Characteristic_Integer(int character, int index);
// returns a bounded integer characteristic
int Characteristic_BInteger(int character, int index, int min, int max);
// returns a string characteristic
void Characteristic_String(int character, int index, char *buf, int size);
// free cached bot characters
void BotShutdownCharacters(void);
