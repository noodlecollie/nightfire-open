﻿// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.Events
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Mdl
{
  public enum Events
  {
    SCRIPT_EVENT_DEAD = 1000, // 0x000003E8
    SCRIPT_EVENT_NOINTERRUPT = 1001, // 0x000003E9
    SCRIPT_EVENT_CANINTERRUPT = 1002, // 0x000003EA
    SCRIPT_EVENT_FIREEVENT = 1003, // 0x000003EB
    SCRIPT_EVENT_SOUND = 1004, // 0x000003EC
    SCRIPT_EVENT_SENTENCE = 1005, // 0x000003ED
    SCRIPT_EVENT_INAIR = 1006, // 0x000003EE
    SCRIPT_EVENT_ENDANIMATION = 1007, // 0x000003EF
    SCRIPT_EVENT_SOUND_VOICE = 1008, // 0x000003F0
    SCRIPT_EVENT_SENTENCE_RND1 = 1009, // 0x000003F1
    SCRIPT_EVENT_NOT_DEAD = 1010, // 0x000003F2
    SCRIPT_EVENT_EMPHASIS = 1011, // 0x000003F3
    SCRIPT_EVENT_BODYGROUPON = 1020, // 0x000003FC
    SCRIPT_EVENT_BODYGROUPOFF = 1021, // 0x000003FD
    SCRIPT_EVENT_BODYGROUPTEMP = 1022, // 0x000003FE
    SCRIPT_EVENT_FIRE_INPUT = 1100, // 0x0000044C
    NPC_EVENT_BODYDROP_LIGHT = 2001, // 0x000007D1
    NPC_EVENT_BODYDROP_HEAVY = 2002, // 0x000007D2
    NPC_EVENT_SWISHSOUND = 2010, // 0x000007DA
    NPC_EVENT_180TURN = 2020, // 0x000007E4
    NPC_EVENT_WEAPON_PICKUP = 2040, // 0x000007F8
    NPC_EVENT_WEAPON_DROP = 2041, // 0x000007F9
    NPC_EVENT_WEAPON_SET_SEQUENCE_NAME = 2042, // 0x000007FA
    NPC_EVENT_WEAPON_SET_SEQUENCE_NUMBER = 2043, // 0x000007FB
    NPC_EVENT_WEAPON_SET_ACTIVITY = 2044, // 0x000007FC
    NPC_EVENT_LEFTFOOT = 2050, // 0x00000802
    NPC_EVENT_RIGHTFOOT = 2051, // 0x00000803
    NPC_EVENT_OPEN_DOOR = 2060, // 0x0000080C
    EVENT_WEAPON_MELEE_HIT = 3001, // 0x00000BB9
    EVENT_WEAPON_SMG1 = 3002, // 0x00000BBA
    EVENT_WEAPON_MELEE_SWISH = 3003, // 0x00000BBB
    EVENT_WEAPON_SHOTGUN_FIRE = 3004, // 0x00000BBC
    EVENT_WEAPON_THROW = 3005, // 0x00000BBD
    EVENT_WEAPON_AR1 = 3006, // 0x00000BBE
    EVENT_WEAPON_AR2 = 3007, // 0x00000BBF
    EVENT_WEAPON_HMG1 = 3008, // 0x00000BC0
    EVENT_WEAPON_SMG2 = 3009, // 0x00000BC1
    EVENT_WEAPON_MISSILE_FIRE = 3010, // 0x00000BC2
    EVENT_WEAPON_SNIPER_RIFLE_FIRE = 3011, // 0x00000BC3
    EVENT_WEAPON_AR2_GRENADE = 3012, // 0x00000BC4
    EVENT_WEAPON_THROW2 = 3013, // 0x00000BC5
    EVENT_WEAPON_PISTOL_FIRE = 3014, // 0x00000BC6
    EVENT_WEAPON_RELOAD = 3015, // 0x00000BC7
    EVENT_WEAPON_THROW3 = 3016, // 0x00000BC8
    EVENT_WEAPON_SMG1_BURST1 = 3101, // 0x00000C1D
    EVENT_WEAPON_SMG1_BURSTN = 3102, // 0x00000C1E
    EVENT_WEAPON_AR2_ALTFIRE = 3103, // 0x00000C1F
    EVENT_WEAPON_SEQUENCE_FINISHED = 3900, // 0x00000F3C
    EVENT_WEAPON_LAST = 3999, // 0x00000F9F
    CL_EVENT_MUZZLEFLASH0 = 5001, // 0x00001389
    CL_EVENT_SPARK0 = 5002, // 0x0000138A
    CL_EVENT_NPC_MUZZLEFLASH0 = 5003, // 0x0000138B
    CL_EVENT_SOUND = 5004, // 0x0000138C
    CL_EVENT_MUZZLEFLASH1 = 5011, // 0x00001393
    CL_EVENT_NPC_MUZZLEFLASH1 = 5013, // 0x00001395
    CL_EVENT_MUZZLEFLASH2 = 5021, // 0x0000139D
    CL_EVENT_NPC_MUZZLEFLASH2 = 5023, // 0x0000139F
    CL_EVENT_MUZZLEFLASH3 = 5031, // 0x000013A7
    CL_EVENT_NPC_MUZZLEFLASH3 = 5033, // 0x000013A9
    CL_EVENT_EJECTBRASS1 = 6001, // 0x00001771
    CL_EVENT_SPRITEGROUP_CREATE = 6002, // 0x00001772
    CL_EVENT_SPRITEGROUP_DESTROY = 6003, // 0x00001773
    CL_EVENT_FOOTSTEP_LEFT = 6004, // 0x00001774
    CL_EVENT_FOOTSTEP_RIGHT = 6005, // 0x00001775
    EVENT_FACEPOSER_TAG = 8001, // 0x00001F41
    CL_EVENT_DISPATCHEFFECT0 = 9001, // 0x00002329
    CL_EVENT_DISPATCHEFFECT1 = 9011, // 0x00002333
    CL_EVENT_DISPATCHEFFECT2 = 9021, // 0x0000233D
    CL_EVENT_DISPATCHEFFECT3 = 9031, // 0x00002347
    CL_EVENT_DISPATCHEFFECT4 = 9041, // 0x00002351
    CL_EVENT_DISPATCHEFFECT5 = 9051, // 0x0000235B
    CL_EVENT_DISPATCHEFFECT6 = 9061, // 0x00002365
    CL_EVENT_DISPATCHEFFECT7 = 9071, // 0x0000236F
    CL_EVENT_DISPATCHEFFECT8 = 9081, // 0x00002379
    CL_EVENT_DISPATCHEFFECT9 = 9091, // 0x00002383
  }
}
