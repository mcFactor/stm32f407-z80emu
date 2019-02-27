#include <string.h>
#include <stdint.h>

#include "startup.h"
#include "Emulator.h"
#include "SDCard.h"
#include "Emulator/z80main.h"
#include "Keyboard/ps2Keyboard.h"

extern "C" void initialize()
{
	initializeVideo();
}

extern "C" void setup()
{
	startVideo();

	// Initialize PS2 Keyboard
	Ps2_Initialize();

	// Initialize Spectrum emulator
	zx_setup(&MainScreen);

	showHelp();
}

extern "C" void loop()
{
	if (loadSnapshotLoop())
	{
		return;
	}

	if (saveSnapshotLoop())
	{
		return;
	}

	if (showKeyboardLoop())
	{
		return;
	}

	if (setDateTimeLoop())
	{
		return;
	}

	int32_t result = zx_loop();
	switch (result)
	{
	case KEY_ESC:
		clearHelp();
		break;

	case KEY_F1:
		toggleHelp();
		break;

#ifndef BOARD2
	case KEY_F2:
		if (!saveSnapshotSetup())
		{
			showErrorMessage("Cannot initialize SD card");
		}
		break;
#endif

	case KEY_F3:
		if (!loadSnapshotSetup())
		{
			showErrorMessage("Error when loading from SD card");
		}
		break;

	case KEY_F5:
		zx_reset();
		showHelp();
		break;

#ifndef BOARD2
	case KEY_F6:
		setDateTimeSetup();
		break;
#endif

	case KEY_F10:
		showKeyboardSetup();
		break;

	case KEY_F12:
		showRegisters();
		break;
	}
}
