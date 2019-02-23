#ifndef IR_CODE_H
#define	IR_CODE_H

typedef struct {
  uint8_t repeatCount;
  uint32_t gap;
  uint16_t codeLength;
  uint16_t* code;
} IrCode;

void ir_code_setup();
IrCode* ir_code_decode(uint16_t* buffer, uint16_t bufferLen);
IrCode* ir_code_getByIndex(uint16_t codeIndex);

#endif // IR_CODE_H
