#ifndef _SALTO_DEFINES_
#define _SALTO_DEFINES_

//#define AUDIO_TO_FILE
#define MONO
//#define CORRECT_PITCH
//#define DEMO_VERSION
                                   
//  GENERAL DEFINES
#define MIDI_DEVICE_NR            0       	// 0 is first input port in OMS
#define MIDI_EXT_CTRL_RANGE       127     	// resolution of external controller
#define MIDI_EXT_CTRL_OFFSET      64      	//
#define MAX_STAT_TEMPLATES        8
#define MAX_SINES									200				// max number of Peaks/Sines
//#define OSCILLOSCOPE_REFRESH_TIME 0.015 	// in secs
#define OSCILLOSCOPE_REFRESH_TIME 0.04    	// in secs = 25 frames/sec
//#define OSCILLOSCOPE_REFRESH_TIME 0.05    	// in secs = 20 frames/sec
//#define OSCILLOSCOPE_REFRESH_TIME 0.1    	// in secs = 10 frames/sec

// math constants
#define HALFPI    (PI/2)

// global macros
#undef MIN
#undef MAX
#define MIN(a,b)      ((a <= b) ? (a) : (b))
#define MAX(a,b)      ((a >= b) ? (a) : (b))
#define CLIP(a,b,c)   ((a<=b)?(b):(a>=c)?(c):(a))

// FlowControl Enums
enum
{
  FLOW_INTERPOLATE = 0,
  FLOW_GENERATE_SINE,
  FLOW_FOLLOW_TRACK,
  FLOW_SDIF_EDIT,
  NO_TRANS_MODE=0,
  INIT_TRANS_MODE,
  DO_TRANSITION
};
#endif




