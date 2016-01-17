/*
 * ringtone.c
 *
 *  Created on: 14/mag/2015
 *      Author: Ghost
 */

#include "stm32f4xx_helpers.h"

/* Private variables */
static int notes[] = { 0,
NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4,
NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5,
NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6,
NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7,
NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7 };

static const char* rtttl_songs[] =
		{
			"Alarm:d=4,o=5,b=40:16a#6,16a#6,16a#6,16a#6,16a#6,16a#6,16a#6,16a#6,16a#6,16a#6",
			"The Bare Necessities:d=4,o=5,b=200:8g,8p,8a,8p,8c6,p,8p,2e6,8d6,8p,8e6,8p,8d6,c6,8p,8c6,8p,8d6,c6,8p,8d6,8p,8c6,8p,8d6,8p,8c6,a,8p,g,c6,8p,8c6,8c6,8p,8e6,8p,8a6,g6,8p,f6,e6,d6,p,2e6,8d6,8p,8g6,8p,8a6,8p,8g6,p,8p,a6,p,8g6,8p,8a6,8p,8g6,e6",
			"Pinky And The Brain:d=4,o=5,b=210:16b.,16p,8e.6,16p,16d#.6,16p,8e.6,16p,16g.6,16p,d#.6,p,16b.,16p,8e.6,16p,16d#.6,16p,8e.6,16p,16g.6,16p,d#.6,p,e6,8p,8e.6,16p,16g.6,16p,8a#.6,16p,a#6,8p,16a#.6,16p,8b.6,16p,16a.6,16p,g6,8p,f#6,p,16e.6,16p,8a.6,16p,16g#.6,16p,8a.6,16p,16b.6,16p,g#6,p,16e.6,16p,8a.6,16p,16g#.6,16p,8a.6,16p,16b.6,16p,g#6,p,16c.6,16p,8b.,16p,8b.,8p,16b.,16p,8b.,16p,16b.,16p,8c#.6,16p,16d#.6,16p,e6,8p,d6,8p,c6,8p,b,8p,e6,8p,16b.,16p,16a#.,16p,16b.,16p,c6,8p,b,8p,e6",
			"Inspector Gadget:d=4,o=5,b=160:8c,16d,8d#,16f,8g,16p,8d#,16p,8f#,16p,8d,16p,8f,16p,8d#,16p,8c,16d,8d#,16f,8g,16p,8c6,16p,2b,p,8c,16d,8d#,16f,8g,16p,8d#,16p,8f#,16p,8d,16p,8f,16p,8d#,16p,8c6,16b,8a#,16a,2g#,8g,16p,2c6",
			"Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6",
			"StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6",
			"Star Wars Imperial:d=4,o=6,b=112:8d.,16p,8d.,16p,8d.,16p,8a#5,16p,16f,8d.,16p,8a#5,16p,16f,d.,8p,8a.,16p,8a.,16p,8a.,16p,8a#,16p,16f,8c#.,16p,8a#5,16p,16f,d.,8p,8d.7,16p,8d,16p,16d,8d7,8p,8c#7,16p,16c7,16b,16a#,8b,8p,16d#,16p,8g#,8p,8g,16p,16f#,16f,16e,8f,8p,16a#5,16p,8c#,8p,8a#5,16p,16c#,8f.,16p,8d,16p,16f,a.,8p,8d.7,16p,8d,16p,16d,8d7,8p,8c#7,16p,16c7,16b,16a#,8b,8p,16d#,16p,8g#,8p,8g,16p,16f#,16f,16e,8f,8p,16a#5,16p,8c#,8p,8a#5,16p,16f,8d.,16p,8a#5,16p,16f,d.",
			"TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5",
			"Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6",
			"smb:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6",
			"Xfiles:d=4,o=5,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b.",
			"Popeye:d=4,o=5,b=140:16g.,16f.,16g.,16p,32p,16c.,16p,32p,16c.,16p,32p,16e.,16d.,16c.,16d.,16e.,16f.,g,8p,16a,16f,16a,16c6,16b,16a,16g,16a,16g,8e,16g,16g,16g,16g,8a,16b,32c6,32b,32c6,32b,32c6,32b,8c6",
			"MahnaMahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.",
			"Mission Impossible:d=4,o=5,b=120:8d,p,8d,p,8d,8f,8p,8g,8p,8d,p,8d,p,8d,8c,8p,8c#,8p,8d,p,8d,p,8d,8f,8p,8g,8p,8d6,8d#6",
			"In The Jungle:d=4,o=5,b=120:1p,1p,32p,g,16a.,32p,8b,8p,a,16b,32p,c6,8b,16p,8a,16p,32p,g,16a,32p,16b.,16p,16d.,8p,32p,16d.,8p,32p,16d.,8p,1d,8p,g,16a.,32p,8b,8p,a,32p,16b,32p,8c.6,16p,16b.,16p,8a.,16p,8g.,32p,16a,16p,16b.,16p,16d.,8p,8d,8p,16d.,8p,32p,1d",
			"Animaniacs:d=4,o=5,b=160:D#6,D6,D#6,F.6,8D#6,D.6,8D#6,C6,2p,8D6,8D#6,F.6,8D#6,D.6,8D#6,A#,2p,8C6,8A#,8G#,8G#,8C6,8D#6,G#6,8p,8G#6,8A#.6,16G#6,8G6,8G#6,F6,8p,8F6,D#6,C7,A#.6,8G#6,G#6",
			"ducktales:d=4,o=5,b=112:8e6,8e6,16p,16g6,8b6,g#6,p,8e6,8d6,8c6,8d6,8e6,8d6,8c6,8d6,8e6,8e6,16p,16g6,8b6,g#6,p,8e6,8d6,8c6,8d6,8e6,8d6,8c6,8g6,8e6,8e6",
			"Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6"
			"GoodBad:d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#"
			"20thCenFox:d=16,o=5,b=140:b,8p,b,b,2b,p,c6,32p,b,32p,c6,32p,b,32p,c6,32p,b,8p,b,b,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,g#,32p,a,32p,b,8p,b,b,2b,4p,8e,8g#,8b,1c#6,8f#,8a,8c#6,1e6,8a,8c#6,8e6,1e6,8b,8g#,8a,2b"
			"Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6"
			"MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b"
			"Muppets:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c"
			"TopGun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#"
			"A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#"
			"Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6"
			"Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#"
			"Gadget:d=16,o=5,b=50:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#"
			"Smurfs:d=32,o=5,b=200:4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8f#,p,8a#,p,4g#,4p,g#,p,a#,p,b,p,c6,p,4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8b,p,8f,p,4f#"
			"LeisureSuit:d=16,o=6,b=56:f.5,f#.5,g.5,g#5,32a#5,f5,g#.5,a#.5,32f5,g#5,32a#5,g#5,8c#.,a#5,32c#,a5,a#.5,c#.,32a5,a#5,32c#,d#,8e,c#.,f.,f.,f.,f.,f,32e,d#,8d,a#.5,e,32f,e,32f,c#,d#.,c#"
			"smb:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6"
			"smb_under:d=4,o=6,b=100:32c,32p,32c7,32p,32a5,32p,32a,32p,32a#5,32p,32a#,2p,32c,32p,32c7,32p,32a5,32p,32a,32p,32a#5,32p,32a#,2p,32f5,32p,32f,32p,32d5,32p,32d,32p,32d#5,32p,32d#,2p,32f5,32p,32f,32p,32d5,32p,32d,32p,32d#5,32p,32d#"
			"smbdeath:d=4,o=5,b=90:32c6,32c6,32c6,8p,16b,16f6,16p,16f6,16f.6,16e.6,16d6,16c6,16p,16e,16p,16c"
			"ducktales:d=4,o=5,b=112:8e6,8e6,16p,16g6,8b6,g#6,p,8e6,8d6,8c6,8d6,8e6,8d6,8c6,8d6,8e6,8e6,16p,16g6,8b6,g#6,p,8e6,8d6,8c6,8d6,8e6,8d6,8c6,8g6,8e6,8e6"
			"Zelda1:d=4,o=5,b=125:a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,16a#.6,16g#6,16f#.6,8g#.6,16f#.6,2f6,f6,8d#6,16d#6,16f6,2f#6,8f6,8d#6,8c#6,16c#6,16d#6,2f6,8d#6,8c#6,8c6,16c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f,a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,c#7,c7,2a6,f6,2f#.6,a#6,a6,2f6,f6,2f#.6,a#6,a6,2f6,d6,2d#.6,f#6,f6,2c#6,a#,c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f"
			"smario2:d=4,o=5,b=125:8g,16c,8e,8g.,16c,8e,16g,16c,16e,16g,8b,a,8p,16c,8g,16c,8e,8g.,16c,8e,16g,16c#,16e,16g,8b,a,8p,16b,8c6,16b,8c6,8a.,16c6,8b,16a,8g,16f#,8g,8e.,16c,8d,16e,8f,16e,8f,8b.4,16e,8d.,c"
			"smb3lvl1:d=4,o=5,b=80:16g,32c,16g.,16a,32c,16a.,16b,32c,16b,16a.,32g#,16a.,16g,32c,16g.,16a,32c,16a,4b.,32p,16c6,32f,16c.6,16d6,32f,16d.6,16e6,32f,16e6,16d.6,32c#6,16d.6,16c6,32f,16c.6,16d6,32f,16d6,4e.6,32p,16g,32c,16g.,16a,32c,16a.,16b,32c,16b,16a.,32g#,16a.,16c6,8c.6,32p,16c6,4c.6"
			"shinobi:d=4,o=5,b=140:b,f#6,d6,b,g,f#,e,2f#.,a,1f#,p,b,f#6,d6,b,g,f#,e,1f#.,8a,1b.,8a,1f#.,8a,1b.,8a,1f#."
			"outrun_magic:d=4,o=5,b=160:f6,d#6,8g#.6,f6,d#6,8c#.6,d#6,c6,2g#.,c#6,c6,8d#.6,c#6,c6,8f.,a#,16c.6,1a#,f6,d#6,8g#.6,f6,d#6,8c#.6,d#6,c6,2g#.,c#6,c6,8d#.6,c#6,c6,16f.,16g#.,c6,2a#."
			"Popeye:d=4,o=5,b=140:16g.,16f.,16g.,16p,32p,16c.,16p,32p,16c.,16p,32p,16e.,16d.,16c.,16d.,16e.,16f.,g,8p,16a,16f,16a,16c6,16b,16a,16g,16a,16g,8e,16g,16g,16g,16g,8a,16b,32c6,32b,32c6,32b,32c6,32b,8c6"
			"Wonderboy:d=4,o=5,b=225:f6,d6,f6,8d6,f6,32p,8f6,d6,f6,d6,e6,c6,e6,8c6,e6,32p,8e6,c6,e6,c6"
			"smwwd1:d=4,o=5,b=125:a,8f.,16c,16d,16f,16p,f,16d,16c,16p,16f,16p,16f,16p,8c6,8a.,g,16c,a,8f.,16c,16d,16f,16p,f,16d,16c,16p,16f,16p,16a#,16a,16g,2f,16p,8a.,8f.,8c,8a.,f,16g#,16f,16c,16p,8g#.,2g,8a.,8f.,8c,8a.,f,16g#,16f,8c,2c6"
			"dkong:d=4,o=5,b=160:2c,8d.,d#.,c.,16b,16c6,16b,16c6,16b,16c6,16b,16c6,16b,16c6,16b,16c6,16b,2c6"
			"20th Century Fox:d=16,o=5,b=140:b,8p,b,b,2b,p,c6,32p,b,32p,c6,32p,b,32p,c6,32p,b,8p,b,b,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,g#,32p,a,32p,b,8p,b,b,2b,4p,8e,8g#,8b,1c#6,8f#,8a,8c#6,1e6,8a,8c#6,8e6,1e6,8b,8g#,8a,2b"
			"ATeamv1:d=4,o=6,b=125:8f#.7,16f#7,8c#7,1f#7,8a#6,4b6,8c#7,4f#.6,16a#6,16c#7,8f#7,8c#7,8g#7,1f#7,8e.7,16d#7,16c#7,8b.6,2c#7,8f#.7,16f#7,8c#7,1f#7,8a#6,4b6,8c#7,4f#.6",
			"ATeamv2:d=4,o=6,b=200:16d5,16d#5,16d5,16d#5,16d5,16d#5,16d5,16d5,16d#5,16e5,16f5,16f#5,16g5,8g5,p,8g5,p,8a#5,8p,8c6,8p,8g5,p,8g5,p,8f5,8p,8p,8g5,p,p,8a#5,8p,8c6,8p,8g5,p,p,8f5,8p,8f#5,8p,8a#5,8g5,1d5",
			"ATeamv3:d=4,o=6,b=100:1d6,d6,e6,2f#6,1g6,a6,a5,d6,c#6,8f#6,8f#6,8e6,f#6,e6,f#6,e6,8e6,b6,a6,8f#6,8f#6,8e6,f#6,e6,d6,d6,8e6,2e6,8f#6,8f#6,8e6,f#6,e6,f#6,e6,8e6,b6,a6,8f#6,8f#6,8e6,f#6,e6,d6,d6,8e6,2e6",
			"ATeamv4:d=4,o=6,b=355:2e.7,4b.6,1e.7,4a.6,2b.6,1e.6,8g#.6,8b.6,4e.7,4b.6,4f#.7,1e.7,2d7,8c#.7,8b.6,2a6,1b.6",
			"ATeamv5:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#",
			"Airwolf:d=4,o=6,b=100:e5, 16a5, 16b5, 16d, e, 16g, 16f#, 16d, e, 16g, 16f#, 16d, e, 8d, 16f#, b5, a5, 8g5, 16a5, 8f#5, 16d5, g5, 16c, 16d, 16f, g, 16c, 16b, 16f, g, 16c, 16b, 16f, g, 8f, 16a, d, c, 8b5, 16d, 8a5, 16f5, g5, 16c, 16d, 16f, g, 16c, 16b, 16f",
			"Animaniacs:d=4,o=5,b=160:D#6,D6,D#6,F.6,8D#6,D.6,8D#6,C6,2p,8D6,8D#6,F.6,8D#6,D.6,8D#6,A#,2p,8C6,8A#,8G#,8G#,8C6,8D#6,G#6,8p,8G#6,8A#.6,16G#6,8G6,8G#6,F6,8p,8F6,D#6,C7,A#.6,8G#6,G#6",
			"Bach, Toccata:d=4,o=5,b=125:a,16d6,16a,16e6,16a,16f6,16a,16d6,16a,16e6,16a,16f6,16a,16g6,16a,16e6,16a,16f6,16a,16g6,16a,16a6,16a,16f6,16a,16g6,16a,16a6,16a,16a#6,16a,16g6,16a,16a6,16a,16f6,16a,16g6,16a,16e6,16a,16f6,16a,16d6,16a,16e6,16a,16c#6,16a,2d6",
			"Batman:d=4,o=6,b=160:16e,16a5,16e,16a5,16d#,16a5,16d#,16a5,16d,16a5,16d,16a5,16d#,16a5,16d#,16a5,16e,16a5,16e,16a5,16d#,16a5,16d#,16a5,16d,16a5,16d,16a5,16d#,16a5,16d#,16a5,16e,16a5,16e,16a5,16d#,16a5,16d#,16a5,16d,16a5,16d,16a5,16d#,16a5,16d#,16a5,16e,16a5,16e,16a5,16d#,16a5,16d#,16a5,16d,16a5,16d,16a5,16d#,16a5,16d#,16a5,16a,16d,16a,16d,16g#,16d,16g#,16d,16g,16d,16g,16d,16g#,16d,16g#,16d,16a,16d,16a,16d,16g#,16d,16g#,16d,16g,16d,16g,16d,16g#,16d,16g#,16d,16e,16a5,16e,16a5,16d#,16a5,16d#,16a5,16d,16a5,16d,16a5,16d#,16a5,16d#,16a5,16e,16a5,16e,16a5,16e,16a5,16e,2a5",
			"BattleStar Galactica:d=4,o=5,b=120:8e,16e,16e,8b,16b,16b,8e6,16e6,16e6,16d6,16c#6,8b,8d6,16d6,16d6,16c#6,16b,8a,8c#6,8a,b,8e,16e,16e,8b,16b,16b,8e6,16e6,16e6,16d6,16c#6,8b,8e6,16e6,16e6,16d6,16c#6,8b,8c#6,8d6,8a.,16c#6,8b,e",
			"Beep:d=4,o=7,b=50:c",
			"Beethoven, Elise:d=4,o=5,b=125:8e6,8d#6,8e6,8d#6,8e6,8b,8d6,8c6,a,8p,8c,8e,8a,b,8p,8e,8g#,8b,c6,p,8e,8e6,8d#6,8e6,8d#6,8e6,8b,8d6,8c6,a,8p,8c,8e,8a,b,8p,8e,8c6,8b,2a",
			"Beethoven:d=4,o=5,b=125:16g,16g,16g,16d#,16g#,16g#,16g#,16g,16d#6,16d#6,16d#6,c6,16g,16g,16g,16d,16g#,16g#,16g#,16g,16f6,16f6,16f6,d6,16g6,16g6,16f6,16d#6,16d#,16d#,16f,16g,16g6,16g6,16f6,16d#6,16d#,16d#,16f,16g,16g6,16g6,16f6,8d#6,p,8c6,p,2g6",
			"Beethoven Ode to Joy:d=4,o=6,b=100:a5,a5,a_5,c,c,a_5,a5,g5,f5,f5,g5,a5,a.5,8g5,2g5,a5,a5,a_5,c,c,a_5,a5,g5,f5,f5,g5,a5,g.5,8f5,2f5,g5,g5,a5,f5,g5,8a5,8a_5,a5,f5,g5,8a5,8a_5,a5,g5,f5,g5,c5,2a5,a5,a_5,c,c,a_5,a5,g5,f5,f5,g5,a5,g.5,8f5,2f5",
			"Beverley Hills Cop:d=4,o=5,b=100:g6,8a#.6,16g6,16p,16g6,8c7,8g6,8f6,g6,8d.7,16g6,16p,16g6,8d#7,8d7,8a#6,8g6,8d7,8g7,16g6,16f6,16p,16f6,8d6,8a6,2g6",
			"Blue Moon:d=4,o=5,b=100:2p,8p,32p,8d6,8p,2d6,8p,32p,16p,16c6,16p,16d6,32p,16d.6,16p,8d.6,16p,32p,16c.6,d.6,16p,32p,16p,16a,16p,16b,32p,16b,16p,32p,8b,32p,16a,32p,b.,32p,16p,16g,16p,16a,16g,16p,32p,8g,16p,16e,32p,2g",
			"Blue Peter:d=4,o=6,b=220:8b,8a#,8b,8p,8b5,8p,8b5,8p,8f#,8e,8d#,8f#,8b,8a#,8b,8p,8c#7,8c7,8c#7,8p,8c#,8p,8c#,8p,8g#,8f#,8f,8g#,8c#7,8c7,8c#7,8p,8d#7,8e7,8f#7,8e7,8d#7,8c#7,8e7,8d#7,8c#7,8b,8d#7,8c#7,8b,8a,8c#7,8b,8a,8g#,8b,8a,8g#,8f#,8a,8g#,8f#,8e,8d#,8d,8c#,8c,8b5,8p,8b5,8c#,8e,8p,8e,8p,8e,8f#,8g#,8a,8b,8p,8b,8p,8b,8p,8c#7,8d#7,8e7,8f#7,8e7,8c#7,8b,8c#7,8b,8g#,8f#,8e,8c#,8e,8f#,8p,8b5,8c#,8e,8f#,8e,8f#,8e,8c#7,8b,8g#,8g#,8f#,8f#,8g#,8e,8p,c#7,8b,8c#7,8g#,8f#,8e,8p,8g#,8a,8b,8p,8g#,8p,8e",
			"Charlies Angels:d=4,o=5,b=160:d.6,8d#6,2d.6,c6,f6,d#6,d.6,8d#6,2d.6,8d#6,8c6,8d6,8d#6,8f6,g.6,8g#6,2g.6,f6,a#6,g#6,g.6,8g#6,1g6,g#6,a#6,c.7,8c#7,2c.7,a#6,8g.6,8g#.6,8a#.6,c.7,8c#7,2c.7,a#6,d#6,c#7,1c.7",
			"Concerto:d=4,o=5,b=100:8e6,16a,16b,16c#6,16d6,8e6,8a,8a,8f#6,16d6,16e6,16f#6,16g#6,8a6,8a,8a,8d6,16e6,16d6,16c#6,16b,8c#6,16d6,16c#6,16b,16a,8g#,16a,16b,16c#6,16a,32c#6,32d6,32c#6,32d6,32c#6,b,8e6,16a,16b,16c#6,16d6,8e6,8a,8a,8f#6,16d6,16e6,16f#6,16g#6",
			"Madonna: Don't Cry For Me Argentina:d=4,0=5,b=100:4d2,4d2,8d2,4d2,4#d2,8f2,4g2,1f2,8f2,4g2,4g2,8f2,4#a2,4f2,4#d2,2d2,8d2,8#d2,8f2,4c2,2c2,8c2,8d2,8#d2,4#a1,2#a1,8#a1,8c2,8#a1,4d2,2f2,8f1,8f1,8f1,4g1,1#a1,32#c2,4d2,4d2,8d2,2d2,8d2,8d2,4d2,8d2,4d2,4c2,8#a1,4c2,1f1",
			"Downtown:d=4,o=5,b=140:8c6, 8c6, 8c6, c6, 8c6, 8c6, 8c6, 8d6, 8f6, 8d6, c6, 8e6, 8g6, 8e6, 8f6, a6, 2c6, p, 8p, f6, c.6, 8p, 8c6, 8c6, 8c6, c6, 8c6, 8c6, 8c6, 8d6, 8f6, 8d6, e6, 8f6, 8g6, 8e6, 8f6, a6, 2c6, p, 8p, f6, c.6",
			"ET:d=4,o=5,b=90:16f6,16f#6,16f6,16f#6,16a#6,16b6,16f6,16f#6,16b6,16a#6,16b6,16f#6,16g6,16f#6,16g6,16a#6,16b6,16f#6,16g6,g#6,d#7,16c#7,16c7,16a#6,16c7,g#6,2d#6,f6,f7,16d#7,16d7,16c7,16d7,a#6,2g7,a#6,g#7,16g7,16f7,16d#7,16c#7,b6,2g#6,b6,g#6,8g#.7,32g7,g6",
			"Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6",
			"Flintstones 2:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6",
			"Flintstones:d=4,o=6,b=160:e,8a5,8p,a,8f#,e,a5,8p,e,8d,8c#,8c#,8d,8e,a5,b5,2c#.,p,e,a5,8p,a,8f#,e,a5,8p,8e,8e,8d,8c#,8c#,8d,8e,a5,b5,2a.5,p,g#,c#,8p,a,8g#,8g#,8f#,8f,8g#,f#.,8p,f#,b5,8p,8g#,8g#,8f#,8f#,8e,8d#,8f#,e.,8p,e,a5,8p,a,8f#,e,a5,8p,e,8d,8c#,8c#,8d,8e,a5,b5,8p,8c#,8d,8e,a5,b5,8p,8c#,8d,8e,a,b,2a.",
			"Ghostbusters:d=4,o=5,b=125:16c7, 16c7, 8e7, 16c7, 16p, 8d7, 8a#6, 2p, 16c7, 16c7, 16c7, 16c7, 8a#6, 16c7, 16p, 8a6, p, 16c7, 16c7, 8e7, 16c7, 16p, 8d7, 8a#6, 2p, 16c7, 16c7, 16c7, 16c7, 8a#6, 16d7, 16p, 8c7",
			"Good Bad Ugly 2:d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#",
			"Good Bad Ugly:d=4,o=5,b=125:16a,16d6,16a,16d6,2a,f,g,2d,p,16a,16d6,16a,16d6,2a,f,g,2c6,p,16a,16d6,16a,16d6,2a,f,8e,8d,2c,p,16a,16d6,16a,16d6,2a,g,2d.",
			"Great Escape:d=4,o=5,b=110:8e,8g,p,8g,8e6,16p,16d6,8c6,8a,2p,8d6,d6,8c6,8b,16p,16c6,8b,8a,8g,e,p,8e,8f,8e,8e,g,8p,8g,8e6,16p,16d6,8c6,a,2p,8d6,d6,16c6,16p,8b,16p,16c6,8d6,8b,c6",
			"Happy Days:d=4,o=5,b=80:8c#,8f#,8a#,8c#6,16d#6,8f6,2d#6,8d#,8g#,8b,8d#6,16f6,8f#6,2f6,8f,8a#,8d6,8f6,16f#6,8g#6,2f#6,16d#6,8d#6,2f6,16d#6,8d#6,2f6,16f6,16f6,8f6,8f#6,16g6,2g#6,8f#6,8f#6,16g#6,a#6,8g#6,8f#6,16g#6,f#6,8d#6,8d#6,16d#6,2f6,8f#6,8f#6,16g#6,a#6,8g#6",
			"Haunted House:d=4,o=5,b=108: 2a4, 2e, 2d#, 2b4, 2a4, 2c, 2d, 2a#4, 2e., e, 1f4, 1a4, 1d#, 2e., d, 2c., b4, 1a4, 1p, 2a4, 2e, 2d#, 2b4, 2a4, 2c, 2d, 2a#4, 2e., e, 1f4, 1a4, 1d#, 2e., d, 2c., b4, 1a4",
			"Hawaii 5 0:d=4,o=6,b=180:8f#,16f#,16p,16a,16p,8c#.7,16p,b.,16p,2f#.,8p,8f#,16f#,16p,16e,16p,8a.,16p,1f#,p,8f#,16f#,16p,16a,16p,8c#.7,16p,b.,16p,2f#.7,8p,8e7,16e7,16p,16c#7,16p,8a.,16p,1f#.7,2p,8a7,8p,8f#7,16e7,16p,8d7,8b,8f#,16e,16p,8d,2b.5,8p,8b7,8p,8g#7,16f#7,16p,8e7,8c#7,8b,8a,8g,a,8b,c#7,b.,2f#.7,8p,8e7,16e7,16p,16c#7,16p,8a.,16p,1b",
			"In The Jungle:d=4,o=5,b=120:1p, 1p, 32p, g, 16a., 32p, 8b, 8p, a, 16b, 32p, c6, 8b, 16p, 8a, 16p, 32p, g, 16a, 32p, 16b., 16p, 16d., 8p, 32p, 16d., 8p, 32p, 16d., 8p, 1d, 8p, g, 16a., 32p, 8b, 8p, a, 32p, 16b, 32p, 8c.6, 16p, 16b., 16p, 8a., 16p, 8g., 32p, 16a, 16p, 16b., 16p, 16d., 8p, 8d, 8p, 16d., 8p, 32p, 1d",
			"Indy:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6",
			"Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6",
			"Inspector Gadget 2:d=16,o=5,b=50:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#",
			"Inspector Gadget:d=4,o=5,b=160:8c,16d,8d#,16f,8g,16p,8d#,16p,8f#,16p,8d,16p,8f,16p,8d#,16p,8c,16d,8d#,16f,8g,16p,8c6,16p,2b,p,8c,16d,8d#,16f,8g,16p,8d#,16p,8f#,16p,8d,16p,8f,16p,8d#,16p,8c6,16b,8a#,16a,2g#,8g,16p,2c6",
			"James Bond 2:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6",
			"James Bond:d=4,o=5,b=140:8c, 16d, 16d, 8d, d, 8c, 8c, 8c, 8c, 16d#, 16d#, 8d#, d#, 8d, 8d, 8d, 8c, 16d, 16d, 8d, d, 8c, 8c, 8c, 8c, 16d#, 16d#, 8d#, d#, 8d, 8d, 8d, 8c, 16d, 16d, 8d, d, 8c, 8c, 8c, 8c, 16d#, 16d#, 8d#, d#, 8d, 8c#, 8c, 8b, 2a#, 8p, 8g, 8f, 1g",
			"Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#",
			"Jetsons:d=4,o=6,b=180:16b5,16c#,16d#,16e,16f#,16e,16d#,16e,16f#,16e,16f#,16g#,16a,16g#,16f#,16g#,16a,16g#,16a,16b,16c#7,16b,16c#7,16d#7,16e7,16d#7,16e7,16f#7,16g#7,16f#7,16g#7,16a7,16b7,16b,16c#7,16d#7,16e7,16f#7,16g#7,16a7,16a#7,16e7,16e,16e7,16e,16g#7,16g#,8p,16a#7,16a#,16b7,16b,16b7,16b,8p,8e,8p,8b5,8p,8e,8p,8b5,8p,8f,8p,8c,8p,16f7,16c7,16a,16f,8d#,8p,16f,16g,16a,16b,16d7,16c7,16b,16a,16b,16a,16g,16f,16e,16d,16c,16b5,16c,8a5,8p,8c,8p,8f,8p,8c,16f7,16f,16f7,16f,16a7,16a,8p,16b7,16b,16c7,16c7,16c7,16c7,16c7,8p,8f,8p,8c,8p,8f,8p,16f7,16f,16f7,16p,16f#7,16f#,16f#7,16f#,2p,f#.,a#.,c7,c#.7,2p,16d#7,16e7,16f7,16f#7,16f#",
			"Knight Rider:d=4,o=5,b=125:16e,16p,16f,16e,16e,16p,16e,16e,16f,16e,16e,16e,16d#,16e,16e,16e,16e,16p,16f#,16e,16e,16p,16f,16e,16f,16e,16e,16e,16d#,16e,16e,16e,16d,16p,16e,16d,16d,16p,16e,16d,16e,16d,16d,16d,16c,16d,16d,16d,16d,16p,16e,16d,16d,16p,16e,16d,16e,16d,16d,16d,16c,16d,16d,16d",
			"Leisure Suit:d=16,o=6,b=56:f.5,f#.5,g.5,g#5,32a#5,f5,g#.5,a#.5,32f5,g#5,32a#5,g#5,8c#.,a#5,32c#,a5,a#.5,c#.,32a5,a#5,32c#,d#,8e,c#.,f.,f.,f.,f.,f,32e,d#,8d,a#.5,e,32f,e,32f,c#,d#.,c#",
			"Looney Tunes:d=4,o=5,b=160:16f7,16f7,8e7,8d7,8c7,a6,8p,8c7,16f7,16f7,8e7,8d7,8d#7,e7,8p,8e7,8e7,8c7,8d7,8c7,16e7,16e7,8c7,8d7,8a6,8c7,8g6,8a#6,8a6,8f6",
			"Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f",
			"MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b",
			"MahnaMahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.",
			"Mission Impossible 2:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d",
			"Mission Impossible:d=4,o=5,b=120:8d,p,8d,p,8d,8f,8p,8g,8p,8d,p,8d,p,8d,8c,8p,8c#,8p,8d,p,8d,p,8d,8f,8p,8g,8p,8d6,8d#6",
			"Mozart:d=4,o=5,b=63:16f#,16e,16d#,16e,g,16a,16g,16f#,16g,b,16c6,16b,16a#,16b,16f#6,16e6,16d#6,16e6,16f#6,16e6,16d#6,16e6,g6,8e6,8g6,32d6,32e6,16f#6,8e6,8d6,8e6,32d6,32e6,16f#6,8e6,8d6,8e6,32d6,32e6,16f#6,8e6,8d6,8c#6,b",
			"Muppets:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c",
			"Pinky And The Brain:d=4,o=5,b=210:16b.,16p,8e.6,16p,16d#.6,16p,8e.6,16p,16g.6,16p,d#.6,p,16b.,16p,8e.6,16p,16d#.6,16p,8e.6,16p,16g.6,16p,d#.6,p,e6,8p,8e.6,16p,16g.6,16p,8a#.6,16p,a#6,8p,16a#.6,16p,8b.6,16p,16a.6,16p,g6,8p,f#6,p,16e.6,16p,8a.6,16p,16g#.6,16p,8a.6,16p,16b.6,16p,g#6,p,16e.6,16p,8a.6,16p,16g#.6,16p,8a.6,16p,16b.6,16p,g#6,p,16c.6,16p,8b.,16p,8b.,8p,16b.,16p,8b.,16p,16b.,16p,8c#.6,16p,16d#.6,16p,e6,8p,d6,8p,c6,8p,b,8p,e6,8p,16b.,16p,16a#.,16p,16b.,16p,c6,8p,b,8p,e6",
			"Rocky:d=4,o=5,b=112:8d,8e,8f,8p,8f,16f,8f,16p,8e,8d,8c,8c,8d,8e,8d,8p,8d,8e,8f,16p,32p,8e,8f,8g,16p,32p,8f,16p,32a,8p,16p,2a,p,8d,16c,8d,16p,8c",
			"Smurfs:d=32,o=5,b=200:4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8f#,p,8a#,p,4g#,4p,g#,p,a#,p,b,p,c6,p,4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8b,p,8f,p,4f#",
			"Spiderman:d=4,o=5,b=200:8c, 8p, 8d#, g., p, 8f#, 8p, 8d#, c, p, 8p, 8c, 8p, 8d#, 8g., 16p, 8g#, 8g, 8p, 8f#, 8p, 8d#, c, p, 8p, 8f, 8p, 8g#, 2c6, 8p, 8b., 16p, 8g#, f, p, 8p, 8c, 8p, 8d#, g., p, 8f#, 8p, 8d#, 8c., 8p, 16p, 8g#., g, 2p, p, 16p, 8f#, 16p, 8f, 16p, 8d#, f, 8d#, 2c",
			"Star Wars:d=4,o=5,b=225:2c,1f,2g.,8g#,8a#,1g#,2c.,c,2f.,g,g#,c,8g#.,8c.,8c6,1a#.,2c,2f.,g,g#.,8f,c.6,8g#,1f6,2f,8g#.,8g.,8f,2c6,8c.6,8g#.,8f,2c,8c.,8c.,8c,2f,8f.,8f.,8f,2f",
			"Terminator:d=4,o=5,b=125:16d6,8e6,1f6,8e.6,8c6,1f,16p,16d6,8e6,1f6,8e.6,8c6,2a.6,2g.6,8d6,8e6,8f6,1f6,8e.6,8c6,1g.",
			"The Bare Necessities:d=4,o=5,b=200:8g,8p,8a,8p,8c6,p,8p,2e6,8d6,8p,8e6,8p,8d6,c6,8p,8c6,8p,8d6,c6,8p,8d6,8p,8c6,8p,8d6,8p,8c6,a,8p,g,c6,8p,8c6,8c6,8p,8e6,8p,8a6,g6,8p,f6,e6,d6,p,2e6,8d6,8p,8g6,8p,8a6,8p,8g6,p,8p,a6,p,8g6,8p,8a6,8p,8g6,e6",
			"The Muppet Show:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c",
			"Tron:d=4,o=5,b=100:8f6,8c6,8g,e,8p,8f6,8c6,8g,8f6,8c6,8g,e,8p,8f6,8c6,8g,e.,2d",
			"When the saints:d=4,o=5,b=100:8p,16p,32p,16p,16c,32p,32f,16p,4g,16p,32p,32c,32p,32e,32p,16f,32p,4g,16p,16c,16e,16f,32p,8g,32p,8e,16p,8c,8p,16p,4d,16p,32e,32p,16e,32p,16d,8c,16p,32p,16c,8e,32p,16g.,16p,32p,16g,32p,4f,16p,",
			"Whole New World:d=4,o=5,b=240:f#,2g,2b,1a.,2a,2a.,f#,g.,8p,b.,8p,2a,e,2g,f#,2f#.,p,f#,p,f#.,g.,a,2c#6,b,2a.,p,d,2c#6,d6,2a.,p,d,2f#,2e,2d,2e,2g,2f#,2d.,c#,1d.,2p,f#,2g,2b,1a.,2a,2a.,f#,g.,8p,b.,8p,2a,e,2g,f#,2f#.,p,f#,p,f#.,g.,a,2c#6,b,2a.,p,d,2c#6,d6,2a.,p,d,2f#,2e,2d,2e,2g,2f#,2d.,c#,1d.",
			"X Files:d=4,o=5,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b.",
			"XMEN:d=4,o=5,b=280:8g,8c6,8d6,8d#6,2d6,c6,g,2p,8g,8c6,8d6,8d#6,2d6,c6,g#,2p,8g,8c6,8d6,8d#6,2d6,c6,d#6,1p,d6,c6,g,8a,8b,c6,p,8g,8f6,8g6,8g#6,2g6,f6,c6,2p,8c6,8f6,8g6,8g#6,2g6,f6,c#6,2p,8g,8c6,8d6,8d#6,2d6,c6,d#6,1p,d6,c6,g,8a,8b,2c6,2p,8g#.,8g.,8f,8g#.,8g.,8f,8g#.,8g.,8f,8g#.,8g.,8f,8a#.,8g#.,8g,8a#.,8g#.,8g,8a#.,8g#.,8g,8a#.,8g#.,8g,8c.6,8a#.,8g#,8c.6,8a#.,8g#,8c.6,8a#.,8g#,c6,a#,g#,a#,c6,g#6,1g6",
			"Yankee Doodle:d=4,o=5,b=120:8g,8g,8a,8b,8g,8b,8a,8g,8g,8a,8b,8p,f#,8p,8g,8g,8a,8b,8c6,8b,8a,8g,8f#,8d,8e,8f#,8g,g",
			"Zip A Dee Doo Dah:d=4,o=5,b=100:2p,p,16p,16e,16p,32p,16p,16e,32p,8g,16p,c6,16p,32p,16a.,16p,16f,32a,16p,32p,8g.",
			"Beethoven 5th:d=4,o=6,b=100:16g5,16g5,16g5,d_5,16f5,16f5,16f5,d5,16g5,16g5,16g5,16d_5,16g_5,16g_5,16g_5,16g5,16d_,16d_,16d_,c,16g5,16g5,16g5,16d5,16g_5,16g_5,16g_5,16g5,16f,16f,16f,d",
			"MJ Billie Jean:d=4,o=6,b=125:8e,8e,8d,p,8b5,32e,16p,32p,e,d,2p,32e,16p,32p,32e,16p,32p,8e,8d,p,8b5,8e,8g,16p,32p,a,8g,8f_,2e,8p,16p,8e,8p,8e,b,a,8e,c,2b5,16b,16g,16e,16c,2b5",
			"Harry Potter:d=4,o=6,b=160:c5,2f5,g5,2f5,c,2a_.5,2g.5,f.5,8g5,f5,2d_.5,1c5,p,c5,2f5,g5,2f5,c,2d_,d,2c_,a5,2c_,8c,1b5,g_5,1f5,p,c5,2f5,g5,2f5,c,2a_.5,2g.5,f.5,8g5,f5,2d_.5,1c5,p,c5,2f5,g5,2f5,c,2d_,d,2c_,a5,2c_,8c,1b5,g_5,1f5",
			"Bach Air G String:d=4,o=6,b=40:1a5,8p,16d,16a_5,16g5,16f5,16e5,16f5,e.5,8p,2c,16a5,16a5,16d_5,16d5,16g5,16f_5,16c,16a_5,8a_5,16g5,16f5,16g5,16a5,16a_5,16g5,16c5,16g5,16d5,16c5,16f5,16e5,16a_5,16a5,a.5,16b5,16c,8f5,16g5,16a5,16a5,16g5,16g5,16f5,16e5,16d5,8e5,16f5,8f5,16e5,16d5,2c5",
			"Zelda1:d=4,o=5,b=125:a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,16a#.6,16g#6,16f#.6,8g#.6,16f#.6,2f6,f6,8d#6,16d#6,16f6,2f#6,8f6,8d#6,8c#6,16c#6,16d#6,2f6,8d#6,8c#6,8c6,16c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f,a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,c#7,c7,2a6,f6,2f#.6,a#6,a6,2f6,f6,2f#.6,a#6,a6,2f6,d6,2d#.6,f#6,f6,2c#6,a#,c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f",
			"smario2:d=4,o=5,b=125:8g,16c,8e,8g.,16c,8e,16g,16c,16e,16g,8b,a,8p,16c,8g,16c,8e,8g.,16c,8e,16g,16c#,16e,16g,8b,a,8p,16b,8c6,16b,8c6,8a.,16c6,8b,16a,8g,16f#,8g,8e.,16c,8d,16e,8f,16e,8f,8b.4,16e,8d.,c",
			"NULL" };

/* Imported functions */

void play_tone(uint32_t frequency, uint32_t duration) {
	buzz(frequency, duration);
}

void play_pause(uint32_t duration) {
	buzz_sleep(duration);
}

void play_rtttl(char *p) {
	// Absolutely no error checking in here
	uint8_t default_dur = 4;
	uint8_t default_oct = 6;
	int bpm = 63;
	int num;
	long wholenote;
	long duration;
	uint8_t note;
	uint8_t scale;
	// format: d=N,o=N,b=NNN:
	// find the start (skip name, etc)
	while (*p != ':')
		p++;    // ignore name
	p++;                     // skip ':'
	// get default duration
	if (*p == 'd') {
		p++;
		p++;              // skip "d="
		num = 0;
		while (isdigit(*p)) {
			num = (num * 10) + (*p++ - '0');
		}
		if (num > 0)
			default_dur = num;
		p++;                   // skip comma
	}

	// get default octave
	if (*p == 'o') {
		p++;
		p++;              // skip "o="
		num = *p++ - '0';
		if (num >= 3 && num <= 7)
			default_oct = num;
		p++;                   // skip comma
	}
	// get BPM
	if (*p == 'b') {
		p++;
		p++;              // skip "b="
		num = 0;
		while (isdigit(*p)) {
			num = (num * 10) + (*p++ - '0');
		}
		bpm = num;
		p++;                   // skip colon
	}

	// BPM usually expresses the number of quarter notes per minute
	wholenote = (60 * 1000L / bpm) * 4; // this is the time for whole note (in milliseconds)

	// now begin note loop
	while (*p) {
		// first, get note duration, if available
		num = 0;
		while (isdigit(*p)) {
			num = (num * 10) + (*p++ - '0');
		}

		if (num)
			duration = wholenote / num;
		else
			duration = wholenote / default_dur; // we will need to check if we are a dotted note after

		// now get the note
		note = 0;

		switch (*p) {
		case 'c':
			note = 1;
			break;
		case 'd':
			note = 3;
			break;
		case 'e':
			note = 5;
			break;
		case 'f':
			note = 6;
			break;
		case 'g':
			note = 8;
			break;
		case 'a':
			note = 10;
			break;
		case 'b':
			note = 12;
			break;
		case 'p':
		default:
			note = 0;
		}
		p++;

		// now, get optional '#' sharp
		if (*p == '#') {
			note++;
			p++;
		}

		// now, get optional '.' dotted note
		if (*p == '.') {
			duration += duration / 2;
			p++;
		}

		// now, get scale
		if (isdigit(*p)) {
			scale = *p - '0';
			p++;
		} else {
			scale = default_oct;
		}
		scale += OCTAVE_OFFSET;
		if (*p == ',')
			p++;       // skip comma for next note (or we may be at the end)

		// now play the note
		if (note) {
			play_tone(notes[(scale - 4) * 12 + note], duration);
			play_pause(duration * 0.2 * 1000 / 2);
		}
	}
}

void play_song(uint32_t i) {
	play_rtttl(rtttl_songs[i]);
}

uint32_t get_song_total_num() {
	uint32_t i = 0;
	while (strcmp(rtttl_songs[i], "NULL") != 0) {
		i++;
	}
	return i - 1;
}

uint8_t get_song_max_title() {
	uint32_t i = 0;
	uint8_t max = 0;
	uint8_t temp = max;
	char *temp_song;
	while (strcmp(rtttl_songs[i], "NULL") != 0) {
		temp_song = rtttl_songs[i];
		temp = 0;
		while (*(temp_song++) != ':') {
			temp++;
		}
		if (temp > max){
			max = temp;
		}
		i++;
	}
	return max;
}

void get_song_name(uint32_t i, char* buf) {
	uint8_t len = 0;
	char* name = rtttl_songs[i];
	while (*name != ':') {
		*(buf++) = *(name++);
		len++;
	}
	*buf = '\0';
}
