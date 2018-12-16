
#ifndef IR_TX_H
#define IR_TX_H

#include "ir_code.h"

void ir_tx_setup();
void ir_tx_start();
void ir_tx_stop();
void ir_tx_send(IrCode* code);
void ir_irq();

#endif // IR_TX_H
