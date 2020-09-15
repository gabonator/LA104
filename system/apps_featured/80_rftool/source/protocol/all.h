#include "protocol/protocol.h"
#include "protocol/weather.h"
#include "protocol/oregon2.h"
#include "protocol/key360.h"
#include "protocol/vw.h"
#include "protocol/nexus.h"
#include "protocol/gate.h"

CWeather weather;
COregon2 oregon;
CKey360 key360;
CVw vw;
CNexus nexus;
CGate gate;
static CProtocol* protocols[] = {&weather, &oregon, &key360, &vw, &nexus, &gate};
