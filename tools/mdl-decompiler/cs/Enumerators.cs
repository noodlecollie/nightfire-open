using System;
using System.IO;
using JbnLib.Lang;
using JbnLib.Qc;

namespace JbnLib.Mdl
{
    public static class Enumerators
    {
        public static MotionFlags ToMotionFlags(string value)
        {
            switch (value.ToLower())
            {
                case "x":
                    return MotionFlags.X;
                case "y":
                    return MotionFlags.Y;
                case "z":
                    return MotionFlags.Z;
                case "xr":
                    return MotionFlags.XR;
                case "yr":
                    return MotionFlags.YR;
                case "zr":
                    return MotionFlags.ZR;
                case "lx":
                    return MotionFlags.LX;
                case "ly":
                    return MotionFlags.LY;
                case "lz":
                    return MotionFlags.LZ;
                case "ax":
                    return MotionFlags.AX;
                case "ay":
                    return MotionFlags.AY;
                case "az":
                    return MotionFlags.AZ;
                case "axr":
                    return MotionFlags.AXR;
                case "ayr":
                    return MotionFlags.AYR;
                case "azr":
                    return MotionFlags.AZR;
                case "":
                    return MotionFlags.None;
                default:
                    return MotionFlags.Invalid;
            }
        }
        public static int LodFlagsToInt(LevelOfDetailFlags value)
        {
            byte temp = 0;
            if ((value & LevelOfDetailFlags.TimesOne) == LevelOfDetailFlags.TimesOne)
                temp = 1;
            if ((value & LevelOfDetailFlags.TimesTwo) == LevelOfDetailFlags.TimesTwo)
                temp = 2;
            
            int output = 0;
            if ((value & LevelOfDetailFlags.PlusOne) == LevelOfDetailFlags.PlusOne)
                output += 1 * temp;
            if ((value & LevelOfDetailFlags.PlusTwo) == LevelOfDetailFlags.PlusTwo)
                output += 2 * temp;
            if ((value & LevelOfDetailFlags.PlusFour) == LevelOfDetailFlags.PlusFour)
                output += 4 * temp;

            if ((value & LevelOfDetailFlags.TimesFive) == LevelOfDetailFlags.TimesFive)
                output *= 5;

            return output;
        }
        public static object ConvertActivity(Type from, Type to, object activity)
        {
            try
            {
                switch (to.Name)
                {
                    case "ActivityV10":
                        ActivityV10 act10 = (ActivityV10)activity;
                        return Enum.Parse(from, act10.ToString());
                    case "ActivityV11":
                        ActivityV11 act11 = (ActivityV11)activity;
                        return Enum.Parse(from, act11.ToString());
                    case "ActivityV14":
                        ActivityV14 act14 = (ActivityV14)activity;
                        return Enum.Parse(from, act14.ToString());
                    case "ActivityV44":
                        ActivityV44 act44 = (ActivityV44)activity;
                        return Enum.Parse(from, act44.ToString());
                    default:
                        Messages.ThrowException("Mdl.Enumerators.ConvertActivity(Type, Type, object)", Messages.UPDATE_TO_SUPPORT + to.Name);
                        return ActivityV10.ACT_INVALID;
                }
            }
            catch
            {
                switch (to.Name)
                {
                    case "ActivityV10":
                        return ActivityV10.ACT_INVALID;
                    case "ActivityV11":
                        return ActivityV11.ACT_INVALID;
                    case "ActivityV14":
                        return ActivityV14.ACT_INVALID;
                    case "ActivityV44":
                        return ActivityV44.ACT_INVALID;
                    default:
                        Messages.ThrowException("Mdl.Enumerators.ConvertActivity(Type, Type, object)", Messages.UPDATE_TO_SUPPORT + to.Name);
                        return ActivityV10.ACT_INVALID;
                }
            }
        }
    }

    [Flags]
    public enum LevelOfDetailFlags : int
    {
        None = 0,
        TimesOne = 1,
        TimesTwo = 2,
        TimesFive = 4,
        PlusOne = 256,
        PlusTwo = 512,
        PlusFour = 1024
    }

    public enum Events
    {
        // EVEMT_SCRIPTED
        SCRIPT_EVENT_DEAD = 1000,				// character is now dead
        SCRIPT_EVENT_NOINTERRUPT = 1001,		// does not allow interrupt
        SCRIPT_EVENT_CANINTERRUPT = 1002,		// will allow interrupt
        SCRIPT_EVENT_FIREEVENT = 1003,			// Fires OnScriptEventXX output in the script entity, where XX is the event number from the options data.
        SCRIPT_EVENT_SOUND = 1004,				// Play named wave file (on CHAN_BODY)
        SCRIPT_EVENT_SENTENCE = 1005,			// Play named sentence
        SCRIPT_EVENT_INAIR = 1006,				// Leave the character in air at the end of the sequence (don't find the floor)
        SCRIPT_EVENT_ENDANIMATION = 1007,		// Set the animation by name after the sequence completes
        SCRIPT_EVENT_SOUND_VOICE = 1008,		// Play named wave file (on CHAN_VOICE)
        SCRIPT_EVENT_SENTENCE_RND1 = 1009,		// Play sentence group 25% of the time
        SCRIPT_EVENT_NOT_DEAD = 1010,			// Bring back to life (for life/death sequences)
        SCRIPT_EVENT_EMPHASIS = 1011,			// Emphasis point for gestures
        SCRIPT_EVENT_BODYGROUPON = 1020,		// Turn a bodygroup on
        SCRIPT_EVENT_BODYGROUPOFF = 1021,		// Turn a bodygroup off
        SCRIPT_EVENT_BODYGROUPTEMP = 1022,		// Turn a bodygroup on until this sequence ends
	    SCRIPT_EVENT_FIRE_INPUT = 1100,			// Fires named input on the event handler 

        // EVENT_SHARED
        NPC_EVENT_BODYDROP_LIGHT = 2001,
        NPC_EVENT_BODYDROP_HEAVY = 2002,
        NPC_EVENT_SWISHSOUND = 2010,
        NPC_EVENT_180TURN = 2020,
        NPC_EVENT_WEAPON_PICKUP = 2040,
        NPC_EVENT_WEAPON_DROP = 2041,
        NPC_EVENT_WEAPON_SET_SEQUENCE_NAME = 2042,
        NPC_EVENT_WEAPON_SET_SEQUENCE_NUMBER = 2043,
        NPC_EVENT_WEAPON_SET_ACTIVITY = 2044,
        NPC_EVENT_LEFTFOOT = 2050,
        NPC_EVENT_RIGHTFOOT = 2051, 
	    NPC_EVENT_OPEN_DOOR = 2060, 


        // EVENT_WEAPON
        EVENT_WEAPON_MELEE_HIT = 3001,
        EVENT_WEAPON_SMG1 = 3002,
        EVENT_WEAPON_MELEE_SWISH = 3003,
        EVENT_WEAPON_SHOTGUN_FIRE = 3004,
        EVENT_WEAPON_THROW = 3005,
        EVENT_WEAPON_AR1 = 3006,
        EVENT_WEAPON_AR2 = 3007,
        EVENT_WEAPON_HMG1 = 3008,
        EVENT_WEAPON_SMG2 = 3009,
        EVENT_WEAPON_MISSILE_FIRE = 3010,
        EVENT_WEAPON_SNIPER_RIFLE_FIRE = 3011,
        EVENT_WEAPON_AR2_GRENADE = 3012,
        EVENT_WEAPON_THROW2 = 3013,
        EVENT_WEAPON_PISTOL_FIRE = 3014,
        EVENT_WEAPON_RELOAD = 3015,
        EVENT_WEAPON_THROW3 = 3016,
        EVENT_WEAPON_SMG1_BURST1 = 3101,		// first round in a 3-round burst
        EVENT_WEAPON_SMG1_BURSTN = 3102,		// 2, 3 rounds
	    EVENT_WEAPON_AR2_ALTFIRE = 3103, 
	    EVENT_WEAPON_SEQUENCE_FINISHED = 3900, 
        EVENT_WEAPON_LAST = 3999,

        // EVENT_CLIENT
        CL_EVENT_MUZZLEFLASH0 = 5001,			// Muzzleflash on attachment 0
        CL_EVENT_MUZZLEFLASH1 = 5011,			// Muzzleflash on attachment 1
        CL_EVENT_MUZZLEFLASH2 = 5021,			// Muzzleflash on attachment 2
        CL_EVENT_MUZZLEFLASH3 = 5031,			// Muzzleflash on attachment 3
        CL_EVENT_SPARK0 = 5002,					// Spark on attachment 0
        CL_EVENT_NPC_MUZZLEFLASH0 = 5003,		// Muzzleflash on attachment 0 for third person views
        CL_EVENT_NPC_MUZZLEFLASH1 = 5013,		// Muzzleflash on attachment 1 for third person views
        CL_EVENT_NPC_MUZZLEFLASH2 = 5023,		// Muzzleflash on attachment 2 for third person views
        CL_EVENT_NPC_MUZZLEFLASH3 = 5033,		// Muzzleflash on attachment 3 for third person views
        CL_EVENT_SOUND = 5004,					// Emit a sound 

        // EVENT_UNKNOWN
        CL_EVENT_EJECTBRASS1 = 6001,			// Eject a brass shell from attachment 1
	    CL_EVENT_SPRITEGROUP_CREATE = 6002, 
	    CL_EVENT_SPRITEGROUP_DESTROY = 6003, 
	    CL_EVENT_FOOTSTEP_LEFT = 6004, 
	    CL_EVENT_FOOTSTEP_RIGHT = 6005, 

        // EVENT_FACEPOSER
        EVENT_FACEPOSER_TAG = 8001,

        // EVENT_UNKNOWN
        CL_EVENT_DISPATCHEFFECT0 = 9001,		// Hook into a DispatchEffect on attachment 0
        CL_EVENT_DISPATCHEFFECT1 = 9011,		// Hook into a DispatchEffect on attachment 1
        CL_EVENT_DISPATCHEFFECT2 = 9021,		// Hook into a DispatchEffect on attachment 2
        CL_EVENT_DISPATCHEFFECT3 = 9031,		// Hook into a DispatchEffect on attachment 3
        CL_EVENT_DISPATCHEFFECT4 = 9041,		// Hook into a DispatchEffect on attachment 4
        CL_EVENT_DISPATCHEFFECT5 = 9051,		// Hook into a DispatchEffect on attachment 5
        CL_EVENT_DISPATCHEFFECT6 = 9061,		// Hook into a DispatchEffect on attachment 6
        CL_EVENT_DISPATCHEFFECT7 = 9071,		// Hook into a DispatchEffect on attachment 7
        CL_EVENT_DISPATCHEFFECT8 = 9081,		// Hook into a DispatchEffect on attachment 8
        CL_EVENT_DISPATCHEFFECT9 = 9091			// Hook into a DispatchEffect on attachment 9
    }
    
    [Flags]
    public enum MotionFlags : int
    {
        Invalid = -1,
        None = 0,
        X = 0x00000001,
        Y = 0x00000002,	
        Z = 0x00000004,
        XR = 0x00000008,
        YR = 0x00000010,
        ZR = 0x00000020,
        LX = 0x00000040,
        LY = 0x00000080,
        LZ = 0x00000100,
        AX = 0x00000200,
        AY = 0x00000400,
        AZ = 0x00000800,
        AXR = 0x00001000,
        AYR = 0x00002000,
        AZR = 0x00004000,
    }

    [Flags]
    public enum NodeFlags : int
    {
        REVERSE = 1
    }

    [Flags]
    public enum SequenceFlags : int
    {
        LOOPING = 1
    }

    [Flags]
    public enum TypeFlag : int
    {
        Vehicle = 512,
        Water = 2048,
        MultiplayerCharacter = 8192,
        NonPlayableCharacter = 16384
    }
}
