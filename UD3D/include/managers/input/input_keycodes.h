#pragma once

#define UD_KEY_SPACE              32
#define UD_KEY_APOSTROPHE         39  /* ' */
#define UD_KEY_COMMA              44  /* , */
#define UD_KEY_MINUS              45  /* - */
#define UD_KEY_PERIOD             46  /* . */
#define UD_KEY_SLASH              47  /* / */
#define UD_KEY_0                  48
#define UD_KEY_1                  49
#define UD_KEY_2                  50
#define UD_KEY_3                  51
#define UD_KEY_4                  52
#define UD_KEY_5                  53
#define UD_KEY_6                  54
#define UD_KEY_7                  55
#define UD_KEY_8                  56
#define UD_KEY_9                  57
#define UD_KEY_SEMICOLON          59  /* ; */
#define UD_KEY_EQUAL              61  /* = */
#define UD_KEY_A                  65
#define UD_KEY_B                  66
#define UD_KEY_C                  67
#define UD_KEY_D                  68
#define UD_KEY_E                  69
#define UD_KEY_F                  70
#define UD_KEY_G                  71
#define UD_KEY_H                  72
#define UD_KEY_I                  73
#define UD_KEY_J                  74
#define UD_KEY_K                  75
#define UD_KEY_L                  76
#define UD_KEY_M                  77
#define UD_KEY_N                  78
#define UD_KEY_O                  79
#define UD_KEY_P                  80
#define UD_KEY_Q                  81
#define UD_KEY_R                  82
#define UD_KEY_S                  83
#define UD_KEY_T                  84
#define UD_KEY_U                  85
#define UD_KEY_V                  86
#define UD_KEY_W                  87
#define UD_KEY_X                  88
#define UD_KEY_Y                  89
#define UD_KEY_Z                  90
#define UD_KEY_LEFT_BRACKET       91  /* [ */
#define UD_KEY_BACKSLASH          92  /* \ */
#define UD_KEY_RIGHT_BRACKET      93  /* ] */
#define UD_KEY_GRAVE_ACCENT       96  /* ` */
#define UD_KEY_WORLD_1            161 /* non-US #1 */
#define UD_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define UD_KEY_ESCAPE             256
#define UD_KEY_ENTER              257
#define UD_KEY_TAB                258
#define UD_KEY_BACKSPACE          259
#define UD_KEY_INSERT             260
#define UD_KEY_DELETE             261
#define UD_KEY_RIGHT              262
#define UD_KEY_LEFT               263
#define UD_KEY_DOWN               264
#define UD_KEY_UP                 265
#define UD_KEY_PAGE_UP            266
#define UD_KEY_PAGE_DOWN          267
#define UD_KEY_HOME               268
#define UD_KEY_END                269
#define UD_KEY_CAPS_LOCK          280
#define UD_KEY_SCROLL_LOCK        281
#define UD_KEY_NUM_LOCK           282
#define UD_KEY_PRINT_SCREEN       283
#define UD_KEY_PAUSE              284
#define UD_KEY_F1                 290
#define UD_KEY_F2                 291
#define UD_KEY_F3                 292
#define UD_KEY_F4                 293
#define UD_KEY_F5                 294
#define UD_KEY_F6                 295
#define UD_KEY_F7                 296
#define UD_KEY_F8                 297
#define UD_KEY_F9                 298
#define UD_KEY_F10                299
#define UD_KEY_F11                300
#define UD_KEY_F12                301
#define UD_KEY_F13                302
#define UD_KEY_F14                303
#define UD_KEY_F15                304
#define UD_KEY_F16                305
#define UD_KEY_F17                306
#define UD_KEY_F18                307
#define UD_KEY_F19                308
#define UD_KEY_F20                309
#define UD_KEY_F21                310
#define UD_KEY_F22                311
#define UD_KEY_F23                312
#define UD_KEY_F24                313
#define UD_KEY_F25                314
#define UD_KEY_KP_0               320
#define UD_KEY_KP_1               321
#define UD_KEY_KP_2               322
#define UD_KEY_KP_3               323
#define UD_KEY_KP_4               324
#define UD_KEY_KP_5               325
#define UD_KEY_KP_6               326
#define UD_KEY_KP_7               327
#define UD_KEY_KP_8               328
#define UD_KEY_KP_9               329
#define UD_KEY_KP_DECIMAL         330
#define UD_KEY_KP_DIVIDE          331
#define UD_KEY_KP_MULTIPLY        332
#define UD_KEY_KP_SUBTRACT        333
#define UD_KEY_KP_ADD             334
#define UD_KEY_KP_ENTER           335
#define UD_KEY_KP_EQUAL           336
#define UD_KEY_LEFT_SHIFT         16//340
#define UD_KEY_LEFT_CONTROL       17//341
#define UD_KEY_LEFT_ALT           342
#define UD_KEY_LEFT_SUPER         343
#define UD_KEY_RIGHT_SHIFT        344
#define UD_KEY_RIGHT_CONTROL      345
#define UD_KEY_RIGHT_ALT          346
#define UD_KEY_RIGHT_SUPER        347
#define UD_KEY_MENU               348
#define UD_KEY_LAST               UD_KEY_MENU

#define UD_KEY_LEFT_SHIFT_GLFW	340					// special case

#define UD_MOUSE_LEFT				350
#define UD_MOUSE_RIGHT			351

#define UD_KEY_WIN_SHIFT         16
#define UD_KEY_WIN_CONTROL       17
#define UD_KEY_WIN_ALT		   18
#define UD_KEY_WIN_TAB           9
#define UD_KEY_WIN_CAPSLOCK      20

#define UD_PS_4_CONTROLLER_CROSS			1	//0
#define UD_PS_4_CONTROLLER_CIRCLE			2	//1
#define UD_PS_4_CONTROLLER_SQUARE			0	//2
#define UD_PS_4_CONTROLLER_TRIANGLE		3
#define UD_PS_4_CONTROLLER_L1			    4
#define UD_PS_4_CONTROLLER_L2			    6
#define UD_PS_4_CONTROLLER_L3			    10	//9
#define UD_PS_4_CONTROLLER_R1			    5
#define UD_PS_4_CONTROLLER_R2			    7
#define UD_PS_4_CONTROLLER_R3			    11	//10
#define UD_PS_4_CONTROLLER_SHARE			6
#define UD_PS_4_CONTROLLER_OPTIONS		9	//7
#define UD_PS_4_CONTROLLER_PS_BUTTON	    12	//8
#define UD_PS_4_CONTROLLER_DPAD_UP		14	//11
#define UD_PS_4_CONTROLLER_DPAD_RIGHT		15	//12
#define UD_PS_4_CONTROLLER_DPAD_DOWN		16	//13
#define UD_PS_4_CONTROLLER_DPAD_LEFT		17	//14
#define UD_PS_4_CONTROLLER_TRIGGER_L		18			// you manually chose this button code so u can store it in the same buttons vector
#define UD_PS_4_CONTROLLER_TRIGGER_R		19			// you manually chose this button code so u can store it in the same buttons vector

#define UD_XBOX_CONTROLLER_A				0
#define UD_XBOX_CONTROLLER_B				1
#define UD_XBOX_CONTROLLER_X				2
#define UD_XBOX_CONTROLLER_Y				3
#define UD_XBOX_CONTROLLER_L1			    4
#define UD_XBOX_CONTROLLER_R1			    5
#define UD_XBOX_CONTROLLER_WIERD_1		6
#define UD_XBOX_CONTROLLER_WIERD_2		7
#define UD_XBOX_CONTROLLER_L3			    8
#define UD_XBOX_CONTROLLER_R3			    9
#define UD_XBOX_CONTROLLER_DPAD_UP		10
#define UD_XBOX_CONTROLLER_DPAD_RIGHT		11
#define UD_XBOX_CONTROLLER_DPAD_DOWN		12
#define UD_XBOX_CONTROLLER_DPAD_LEFT		13
#define UD_XBOX_CONTROLLER_TRIGGER_L		14			// you manually chose this button code so u can store it in the same buttons vector
#define UD_XBOX_CONTROLLER_TRIGGER_R		15			// you manually chose this button code so u can store it in the same buttons vector