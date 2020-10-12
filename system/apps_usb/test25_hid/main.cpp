#include <library.h>
#include <stm32f10x.h>

// based on https://github.com/pellepl/arcadehid.git
// todo: cleanup code

typedef void (*THandler)(void);

extern "C" 
{
  void _Initialize(void* pDeviceInfo, void* pDevice, void* pDeviceProperty, void* pUserStandardRequests,
        THandler arrHandlerIn[], THandler arrHandlerOut[], THandler callbacks[], THandler pLeaveLowPowerMode)
  {
    BIOS::USB::Initialize(pDeviceInfo, pDevice, pDeviceProperty, pUserStandardRequests, arrHandlerIn, arrHandlerOut, callbacks, pLeaveLowPowerMode);
  }

  void InitializeFinish(int imr_msk)
  {
    BIOS::USB::InitializeFinish(imr_msk);
  }
}

extern "C" {
#include "usb/common.h"

char dbgPushBuf[256];

void dbgPrint(const char* msg)
{
  strcat(dbgPushBuf, msg);
}

}

bool demo = false;

extern "C" void InitUsb();

using namespace BIOS;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{    
  static usb_kb_report kb_report{0};
  static usb_mouse_report mouse_report{0};

  USB_ARC_set_kb_callback([](){});
  USB_ARC_set_mouse_callback([](){});
  USB_ARC_set_joystick_callback([](usb_joystick joy){});

  USB::Enable();
  InitUsb();

  BIOS::DBG::Print("USB HID ready,\nuse encoders to control cursor.\n");

  KEY::EKey key;
  while ((key = KEY::GetKey()) != KEY::Escape)
  {
    if (strlen(dbgPushBuf) > 0)
    {
      BIOS::DBG::Print(dbgPushBuf);
      strcpy(dbgPushBuf, "");
    }

    if (key == KEY::Left)
    {
      mouse_report.dx = -10;
      USB_ARC_MOUSE_tx(&mouse_report);
    }
    else if (key == KEY::Right)
    {
      mouse_report.dx = +10;
      USB_ARC_MOUSE_tx(&mouse_report);
    }
    else if (key == KEY::Up)
    {
      mouse_report.dy = -10;
      USB_ARC_MOUSE_tx(&mouse_report);
    }
    else if (key == KEY::Down)
    {
      mouse_report.dy = +10;
      USB_ARC_MOUSE_tx(&mouse_report);
    }
    else if (key == KEY::F1)
    {
      mouse_report.modifiers = 1<<0;
      USB_ARC_MOUSE_tx(&mouse_report);
    }
    else
    {
      mouse_report.modifiers = 0;
      mouse_report.dx = 0;
      mouse_report.dy = 0;
    }

    if (key == KEY::F3)
    {
      kb_report.modifiers = KB_MOD_NONE;
      kb_report.keymap[0] = KC_G;
      USB_ARC_KB_tx(&kb_report);
      kb_report.keymap[0] = KC_A;
      USB_ARC_KB_tx(&kb_report);
      kb_report.keymap[0] = KC_B;
      USB_ARC_KB_tx(&kb_report);
      kb_report.keymap[0] = KC_O;
      USB_ARC_KB_tx(&kb_report);
      kb_report.keymap[0] = 0;
      USB_ARC_KB_tx(&kb_report);
    } else
    {
      kb_report.modifiers = KB_MOD_NONE;
      kb_report.keymap[0] = 0;
    }

    if (key == KEY::F4)
    {
      demo = !demo; 
    }
    if (demo)
    {
      EVERY(200)
      {
        static int phase = 0;
        mouse_report.dx = 0;
        mouse_report.dy = 0;

        if (phase < 10)
          mouse_report.dx = 10;
        else if (phase < 20)
          mouse_report.dy = 10;
        else if (phase < 30)
          mouse_report.dx = -10;
        else if (phase < 40)
          mouse_report.dy = -10;

        USB_ARC_MOUSE_tx(&mouse_report);

        if (++phase >= 40)
          phase = 0;
      }
    }
  }

  BIOS::USB::InitializeMass();
  return 0;
}
