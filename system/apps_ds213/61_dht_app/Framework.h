#pragma once
#include "../../os_host/source/framework/Wnd.h"
#include "../../os_host/source/framework/Utils.h"
#include "../../os_host/source/framework/Serialize.h"
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/gui/Gui.h"
#include "../../os_host/source/gui/Controls.h"
#include "Sampler.h"

#define DecEnum(e) *((uint8_t*)&(e))-=1
#define IncEnum(e) *((uint8_t*)&(e))+=1
