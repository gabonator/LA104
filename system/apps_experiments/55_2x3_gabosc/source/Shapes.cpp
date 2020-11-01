#include "Shapes.h"

const char CShapes::corner_right[] = 
  "\x10"
  "................"
  "................"
  " ..............."
  "  .............."
  "     ..........."
  "           ....."
  "             ..."
  "              .."
  "               .";

const char CShapes::corner_left[] = 
  "\x10"
  "               ."
  "              .."
  "             ..."
  "           ....."
  "     ..........."
  "  .............."
  " ..............."
  "................"
  "................";

const char CShapes::more_left[] =
  "\x0e"
  "      .       "
  "     ...      "
  "    .....     "
  "   .......    "
  "  .........   "
  " ...........  "
  "              "
  "              ";


const char CShapes::more_right[] =
  "\x0e"
  "              "
  " ...........  "
  "  .........   "
  "   .......    "
  "    .....     "
  "     ...      "
  "      .       "
  "              ";

const char CShapes::updown[] =
  "\x0e"
  "   .      .   "
  "  ..      ..  "
  " ............ "
  "  ..      ..  "
  "   .      .   ";

const char CShapes::leftright[] =
  "\x0e"
  "      .       "
  "     ...      "
  "    .....     "
  "      .       "
  "      .       "
  "      .       "
  "    .....     "
  "     ...      "
  "      .       ";


const char CShapes::cha_base[] =
  "\x09"
  "........."
  "........."
  ".. .... ."
  ".       ."
  "....... ."
  "........."
  "........."
  " ....... "
  "  .....  "
  "   ...   "
  "    .    ";

const char CShapes::trig_base[] =
  "\x09"
  "........."
  ". ......."
  ". ......."
  ".       ."
  ". ......."
  ". ......."
  "........."
  " ....... "
  "  .....  "
  "   ...   "
  "    .    ";

const char CShapes::trig_time[] =
  "\x05"
  ".    "
  "..   "
  "...  "
  ".... "
  "....."
  ".... "
  "...  "
  "..   "
  ".    ";


const char CShapes::chb_base[] =
  "\x09"
  "........."
  ".. ...  ."
  ". ... . ."
  ". .. .. ."
  ". .. .. ."
  "..  ... ."
  "........."
  " ....... "
  "  .....  "
  "   ...   "
  "    .    ";

const char CShapes::chm_base[] =
  "\x09"
  "........."
  ".       ."
  ".. ......"
  "...  ...."
  ".. ......"
  ".       ."
  "........."
  " ....... "
  "  .....  "
  "   ...   "
  "    .    ";

const char CShapes::sel_left[] =
  "\x0e"
  "              "
  "              "
  "              "
  "              "
  "  ..........  "
  " ............ "
  ".............."
  "..............";

const char CShapes::sel_right[] =
  "\x0e"
  ".............."
  ".............."
  " ............ "
  "  ..........  "
  "              "
  "              "
  "              "
  "              ";

const char CShapes::sig_dc[] =
  "\x0e"
  "              "
  "     .  .     "
  "     .  .     "
  "     .  .     "
  "     .  .     "
  "        .     "
  "     .  .     "
  "     .  .     "
  "     .  .     "
  "     .  .     ";

const char CShapes::sig_ac[] =
  "\x0e"
  "              "
  "     .    .   "
  "    .     .   "
  "   .      .   "
  "   .      .   "
  "    .     .   "
  "     .    .   "
  "      .   .   "
  "       .  .   "
  "       .  .   "
  "      .   .   ";

const char CShapes::sig_gnd[] =
  "\x0e"
  "              "
  "      .       "
  "      . .     "
  "      . . .   "
  "      . . .   "
  " ...... . .   "
  "      . . .   "
  "      . . .   "
  "      . .     "
  "      .       "
  "              ";

const char CShapes::sig_dig[] =
  "\x0e"
  "              "
  "         .    "
  "         .    "
  "         .    "
  "    ......    "
  "    .         "
  "    .         "
  "    .         "
  "    ......    "
  "         .    "
  "         .    "
  "         .    ";

const char CShapes::sig_dign[] =
  "\x0e"
  "              "
  "    .         "
  "    .         "
  "    .         "
  "    ......    "
  "         .    "
  "         .    "
  "         .    "
  "    ......    "
  "    .         "
  "    .         "
  "    .         ";

const char CShapes::per_div[] =
  "\x0e"
  "           .. "
  "         ..   "
  "       ..     "
  "              "
  "          ..  "
  "         .  . "
  "       ...... "
  "              "
  "       . .... "
  "              "
  "         ...  "
  "            . "
  "         ...  ";

const char CShapes::trig_pos[] =
  "\x0e"
  "              "
  "            . "
  "        .   . "
  "       .    . "
  "  ........... "
  "  .    .      "
  "  .     .     "
  "  .           "
  "              ";

const char CShapes::trig_pos_[] =
  "\x0e"
  "            . "
  "        .. .x."
  "       .x. .x."
  "  .....x....x."
  " .xxxxxxxxxxx."
  " .x....x..... "
  " .x.   .x.    "
  " .x.    ..    "
  "  .           ";

const char CShapes::trig_neg[] =
  "\x0e"
  "              "
  "  .           "
  "  .   .       "
  "  .    .      "
  "  ........... "
  "       .    . "
  "      .     . "
  "            . "
  "              ";

const char CShapes::trig_neg_[] =
  "\x0e"
  "  .           "
  " .x. ..       "
  " .x. .x.      "
  " .x....x..... "
  " .xxxxxxxxxxx."
  "  .....x....x."
  "     .x.   .x."
  "     ..    .x."
  "            . ";

const char CShapes::trig_level_l[] =
  "\x0e"
  "              "
  " ..           "
  " ..      .    "
  " ..     ...   "
  " ..    .. ..  "
  " ..   ..   .. "
  " ..           "
  " ..           "
  "              ";

const char CShapes::trig_level_g[] =
  "\x0e"
  "              "
  "           .. "
  "           .. "
  " ..   ..   .. "
  "  .. ..    .. "
  "   ...     .. "
  "    .      .. "
  "           .. "
  "              ";

const char CShapes::trig_pulse_ll[] =
  "\x0e"
  "              "
  "        .     "
  "   .    .     "
  "  ...   ..... "
  " .. ..      . "
  "..   .. ..... "
  "        .     "
  "        .     "
  "              ";

const char CShapes::trig_pulse_gl[] =
  "\x0e"
  "              "
  "        .     "
  "        .     "
  "..   .. ..... "
  " .. ..      . "
  "  ...   ..... "
  "   .    .     "
  "        .     "
  "              ";

const char CShapes::trig_pulse_lh[] =
  "\x0e"
  "              "
  "            . "
  "   .        . "
  "  ...   ..... "
  " .. ..  .     "
  "..   .. ..... "
  "            . "
  "            . "
  "              ";

const char CShapes::trig_pulse_gh[] =
  "\x0e"
  "              "
  "            . "
  "            . "
  "..   .. ..... "
  " .. ..  .     "
  "  ...   ..... "
  "   .        . "
  "            . "
  "              ";

const char CShapes::prog_left[] =
  "\x07"
  "......."
  ".     .";

const char CShapes::prog_right[] =
  "\x07"
  ".     ."
  ".......";

const char CShapes::prog_in[] =
  "\x07"
  ".     .";

const char CShapes::prog_pat[] =
  "\x07"
  "   .   "
  "    .  "
  "     . "
  "    .  "
  "   .   "
  "  .    "
  " .     "
  "  .    ";

const char CShapes::prog_sld[] =
  "\x07"
  " ..... "
  " ..... "
  " ..... "
  " ..... "
  " ..... "
  " ..... "
  " ..... "
  " ..... ";


const char CShapes::time_base[] =
  "\x05"
  "..   "
  "...  "
  ".... "
  "....."
  ".... "
  "...  "
  "..   ";


const char CShapes::trig_run[] =
  "\x0e"
  "  .........   "
  "   .......    "
  "    .....     "
  "     ...      "
  "      .       "
  "              "
  "  .........   "
  "  .........   ";

const char CShapes::trig_stop[] =
  "\x0e"
  "    .....     "
  "   .......    "
  "  .........   "
  "  .........   "
  "  .........   "
  "   .......    "
  "    .....     "
  "              ";

const char CShapes::curx1[] =
  "\x0e"
  ".. ..    .    "
  "...     .     "
  "  ...  ...... "
  ".. ..         ";

const char CShapes::cury1[] =
  "\x0e"
  "..       .    "
  ".....   .     "
  "  ...  ...... "
  "..            ";

const char CShapes::curx2[] =
  "\x0e"
  ".. ..  .    . "
  "...   .    .. "
  "  ... .   . . "
  ".. ..  ...  . ";

const char CShapes::cury2[] =
  "\x0e"
  "..      .    ."
  ".....  .    .."
  "  ...  .   . ."
  "..      ...  .";

const char CShapes::minus[] =
  "\x0e"
  "              "
  "      .       "
  "      .       "
  "      .       "
  "      .       "
  "              ";

const char CShapes::markerX[] =
  "\x04" 
  ".   "
  "..  "
  "... "
  "...."
  "... "
  "..  "
  ".   ";

const char CShapes::triangle[] =
  "\x04" 
  "   ."
  "  .."
  " ..."
  "...."
  " ..."
  "  .."
  "   .";

