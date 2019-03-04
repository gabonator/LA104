
#ifndef IR_TX_H
#define IR_TX_H

#include "ir_code.h"

void ir_tx_setup(int carrierFreq = 38000);
void ir_tx_start();
void ir_tx_stop();
void ir_tx_send(IrCode* code);
bool ir_tx_finished();
void ir_irq();

#ifdef EMULATED
void ir_tx_setup(int carrierFreq) {}
void ir_tx_start() {}
void ir_tx_stop() {}
void ir_tx_send(IrCode* code) {}
void ir_irq() {}
bool ir_tx_finished() { return true; }
#endif

#endif // IR_TX_H
