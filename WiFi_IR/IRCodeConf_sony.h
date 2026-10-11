#ifndef IRCODECONF_SONY_H
#define IRCODECONF_SONY_H

typedef struct
{
 uint32_t    sData;
 uint16_t    sAddress;
 uint8_t     sCommand;
 int_fast8_t sRepeats;
 uint8_t     sBits;
} IR_type;

IR_type IR_Key[] = {
                    { // IR_SOURCE [0]
                     .sData = (0x1 << 7) | 0x25,
                     .sAddress = 0x1,
                     .sCommand = 0x25,
                     .sRepeats = 2,
                     .sBits = 12
                    },
                    { // IR_ON_OFF [1]
                     .sData = (0x1 << 7) | 0x15,
//                     .sData = 0xA90,
                     .sAddress = 0x1,
                     .sCommand = 0x15,
                     .sRepeats = 2,
                     .sBits = 12
                    },
                    { // IR_TRACKID [2]
                     .sData = (0x1A << 7) | 0x7E,
                     .sAddress = 0x1A,
                     .sCommand = 0x7E,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_SKIP_BACKWARD [3]
                     .sData = (0x97 << 7) | 0x3C,
                     .sAddress = 0x97,
                     .sCommand = 0x3C,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_PAUSE [4]
                     .sData = (0x97 << 7) | 0x19,
                     .sAddress = 0x97,
                     .sCommand = 0x19,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_SKIP_FORWARD [5]
                     .sData = (0x97 << 7) | 0x3D,
                     .sAddress = 0x97,
                     .sCommand = 0x3D,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_I_MANUAL [6]
                     .sData = (0x1A << 7) | 0x7B,
                     .sAddress = 0x1A,
                     .sCommand = 0x7B,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_BACK [7]
                     .sData = (0x97 << 7) | 0x1B,
                     .sAddress = 0x97,
                     .sCommand = 0x1B,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_PLAY [8]
                     .sData = (0x97 << 7) | 0x1A,
                     .sAddress = 0x97,
                     .sCommand = 0x1A,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_FAST_FORWARD [9]
                     .sData = (0x97 << 7) | 0x1C,
                     .sAddress = 0x97,
                     .sCommand = 0x1C,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_3D [10]
                     .sData = (0x77 << 7) | 0x4D,
                     .sAddress = 0x77,
                     .sCommand = 0x4D,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_SCREEN_SIZE [11]
                     .sData = (0xA4 << 7) | 0x3D,
                     .sAddress = 0xA4,
                     .sCommand = 0x3D,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_STOP [12]
                     .sData = (0x97 << 7) | 0x18,
                     .sAddress = 0x97,
                     .sCommand = 0x18,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_REC [13]
                     .sData = (0x97 << 7) | 0x20,
                     .sAddress = 0x97,
                     .sCommand = 0x20,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_DIGITAL_ANALOG [14]
                     .sData = (0x77 << 7) | 0xD,
                     .sAddress = 0x77,
                     .sCommand = 0xD,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_EXIT [15]
                     .sData = (0x1 << 7) | 0x63,
                     .sAddress = 0x1,
                     .sCommand = 0x63,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_SCENE [16]
                     .sData = (0x1A << 7) | 0x78,
                     .sAddress = 0x1A,
                     .sCommand = 0x78,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_INTERNET_VIDEO [17]
                     .sData = (0x1A << 7) | 0x79,
                     .sAddress = 0x1A,
                     .sCommand = 0x79,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_SYNC_MENU [18]
                     .sData = (0x1A << 7) | 0x58,
                     .sAddress = 0x1A,
                     .sCommand = 0x58,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_GUIDE [19]
                     .sData = (0xA4 << 7) | 0x5B,
                     .sAddress = 0xA4,
                     .sCommand = 0x5B,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_UP [20]
                     .sData = (0x1 << 7) | 0x74,
                     .sAddress = 0x1,
                     .sCommand = 0x74,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_I_PLUS [21]
                     .sData = (0x1 << 7) | 0x3A,
                     .sAddress = 0x1,
                     .sCommand = 0x3A,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_LEFT [22]
                     .sData = (0x1 << 7) | 0x34,
                     .sAddress = 0x1,
                     .sCommand = 0x34,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_OK [23]
                     .sData = (0x1 << 7) | 0x65,
                     .sAddress = 0x1,
                     .sCommand = 0x65,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_RIGHT [24]
                     .sData = (0x1 << 7) | 0x33,
                     .sAddress = 0x1,
                     .sCommand = 0x33,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_RETURN [25]
                     .sData = (0x97 << 7) | 0x23,
                     .sAddress = 0x97,
                     .sCommand = 0x23,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_DOWN [26]
                     .sData = (0x1 << 7) | 0x75,
                     .sAddress = 0x1,
                     .sCommand = 0x75,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_OPTIONS [27]
                     .sData = (0x97 << 7) | 0x36,
                     .sAddress = 0x97,
                     .sCommand = 0x36,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_HOME [28]
                     .sData = (0x1 << 7) | 0x60,
                     .sAddress = 0x1,
                     .sCommand = 0x60,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_1 [29]
                     .sData = (0x1 << 7) | 0x0,
                     .sAddress = 0x1,
                     .sCommand = 0x0,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_2 [30]
                     .sData = (0x1 << 7) | 0x1,
                     .sAddress = 0x1,
                     .sCommand = 0x1,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_3 [31]
                     .sData = (0x1 << 7) | 0x2,
                     .sAddress = 0x1,
                     .sCommand = 0x2,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_4 [32]
                     .sData = (0x1 << 7) | 0x3,
                     .sAddress = 0x1,
                     .sCommand = 0x3,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_5 [33]
                     .sData = (0x1 << 7) | 0x4,
                     .sAddress = 0x1,
                     .sCommand = 0x4,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_6 [34]
                     .sData = (0x1 << 7) | 0x5,
                     .sAddress = 0x1,
                     .sCommand = 0x5,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_7 [35]
                     .sData = (0x1 << 7) | 0x6,
                     .sAddress = 0x1,
                     .sCommand = 0x6,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_8 [36]
                     .sData = (0x1 << 7) | 0x7,
                     .sAddress = 0x1,
                     .sCommand = 0x7,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_9 [37]
                     .sData = (0x1 << 7) | 0x8,
                     .sAddress = 0x1,
                     .sCommand = 0x8,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_TELETEXT [38]
                     .sData = (0x1 << 7) | 0x3F,
                     .sAddress = 0x1,
                     .sCommand = 0x3F,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_0 [39]
                     .sData = (0x1 << 7) | 0x9,
                     .sAddress = 0x1,
                     .sCommand = 0x9,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_SUBTITLES [40]
                     .sData = (0x97 << 7) | 0x28,
                     .sAddress = 0x97,
                     .sCommand = 0x28,
                     .sRepeats = 2,
                     .sBits =  15
                    },
                    { // IR_VOLUME_PLUS [41]
                     .sData = (0x1 << 7) | 0x12,
//                     .sData = 0x490,
                     .sAddress = 0x1,
                     .sCommand = 0x12,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_PROG_PLUS [42]
                     .sData = (0x1 << 7) | 0x10,
//                     .sData = 0x090,
                     .sAddress = 0x1,
                     .sCommand = 0x10,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_MUTE [43]
                     .sData = (0x1 << 7) | 0x14,
                     .sAddress = 0x1,
                     .sCommand = 0x14,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_AUDIO [44]
                     .sData = (0x1 << 7) | 0x17,
                     .sAddress = 0x1,
                     .sCommand = 0x17,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_VOLUME_MINUS [45]
                     .sData = (0x1 << 7) | 0x13,
//                     .sData = 0xC90,
                     .sAddress = 0x1,
                     .sCommand = 0x13,
                     .sRepeats = 2,
                     .sBits =  12
                    },
                    { // IR_PROG_MINUS [46]
                     .sData = (0x1 << 7) | 0x11,
//                     .sData = 0x890,
                     .sAddress = 0x1,
                     .sCommand = 0x11,
                     .sRepeats = 2,
                     .sBits =  12
                    }
                   };

#define IR_SOURCE 0
#define IR_ON_OFF 1
#define IR_TRACKID 2
#define IR_SKIP_BACKWARD 3
#define IR_PAUSE 4
#define IR_SKIP_FORWARD 5
#define IR_I_MANUAL 6
#define IR_BACK 7
#define IR_PLAY 8
#define IR_FAST_FORWARD 9
#define IR_3D 10
#define IR_SCREEN_SIZE 11
#define IR_STOP 12
#define IR_REC 13
#define IR_DIGITAL_ANALOG 14
#define IR_EXIT 15
#define IR_SCENE 16
#define IR_INTERNET_VIDEO 17
#define IR_SYNC_MENU 18
#define IR_GUIDE 19
#define IR_UP 20
#define IR_I_PLUS 21
#define IR_LEFT 22
#define IR_OK 23
#define IR_RIGHT 24
#define IR_RETURN 25
#define IR_DOWN 26
#define IR_OPTIONS 27
#define IR_HOME 28
#define IR_1 29
#define IR_2 30
#define IR_3 31
#define IR_4 32
#define IR_5 33
#define IR_6 34
#define IR_7 35
#define IR_8 36
#define IR_9 37
#define IR_TELETEXT 38
#define IR_0 39
#define IR_SUBTITLES 40
#define IR_VOLUME_PLUS 41
#define IR_PROG_PLUS 42
#define IR_MUTE 43
#define IR_AUDIO 44
#define IR_VOLUME_MINUS 45
#define IR_PROG_MINUS 46

uint8_t decode_IR_Key[] = {
                           IR_0,
                           IR_1,
                           IR_2,
                           IR_3,
                           IR_4,
                           IR_5,
                           IR_6,
                           IR_7,
                           IR_8,
                           IR_9
                          };

String str_IR_Key[] = {
                       "IR_SOURCE",
                       "IR_ON_OFF",
                       "IR_TRACKID",
                       "IR_SKIP_BACKWARD",
                       "IR_PAUSE",
                       "IR_SKIP_FORWARD",
                       "IR_I_MANUAL",
                       "IR_BACK",
                       "IR_PLAY",
                       "IR_FAST_FORWARD",
                       "IR_3D",
                       "IR_SCREEN_SIZE",
                       "IR_STOP",
                       "IR_REC",
                       "IR_DIGITAL_ANALOG",
                       "IR_EXIT",
                       "IR_SCENE",
                       "IR_INTERNET_VIDEO",
                       "IR_SYNC_MENU",
                       "IR_GUIDE",
                       "IR_UP",
                       "IR_I_PLUS",
                       "IR_LEFT",
                       "IR_OK",
                       "IR_RIGHT",
                       "IR_RETURN",
                       "IR_DOWN",
                       "IR_OPTIONS",
                       "IR_HOME",
                       "IR_1",
                       "IR_2",
                       "IR_3",
                       "IR_4",
                       "IR_5",
                       "IR_6",
                       "IR_7",
                       "IR_8",
                       "IR_9",
                       "IR_TELETEXT",
                       "IR_0",
                       "IR_SUBTITLES",
                       "IR_VOLUME_PLUS",
                       "IR_PROG_PLUS",
                       "IR_MUTE",
                       "IR_AUDIO",
                       "IR_VOLUME_MINUS",
                       "IR_PROG_MINUS"
                      };

// necessario per invertire l'ordine dei bit del campo data
uint32_t LSB2MSB(uint32_t n, uint8_t nBit)
{
 uint32_t res = 0;
 for (int i = 0; i < nBit; i++)
 {
  if ((n & (1 << i)))
   res |= (1 << ((nBit - 1) - i));
 }
 return res;
}

void MyIRSend(uint8_t key)
{
 irsend.sendSony(
                 LSB2MSB(IR_Key[key].sData, IR_Key[key].sBits),
                 IR_Key[key].sBits,
                 IR_Key[key].sRepeats
                );
 delay(100);                
}

#endif