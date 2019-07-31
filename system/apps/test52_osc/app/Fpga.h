/********************* (C) COPYRIGHT 2017 e-Design Co.,Ltd. ********************
  DS213_APP Fpga.h                                               Author : bure
*******************************************************************************/
#ifndef __FPGADRV_H
#define __FPGADRV_H


//=================== FPGA控制相关定义 ===================

  #define A_C_CH         0
  #define B_D_CH         1

  //FPGA 控制命令
  #define CHIP_RST       0x80  /* FPGA 初始复位 */


  //FPGA 工作状态标志位定义
  #define SMPL_Din       0x01    /* 数字信号读出   */
  #define PSMPL          0x02    /* 信号预采样完成 */
  #define TRIGG          0x04    /* 触发条件成立   */
  #define FULL           0x08    /* 数据缓冲区写满 */
  #define EMPTY          0x10    /* 数据缓冲区读空 */
  #define TRIGG_ASK      0x20    /* 本地触发请求   */
  #define SCRNF          0x40    /* 满一屏数据     */

  //采样控制参数设置命令
  #define SMPL_MODE      0x40    /* 采样模式设置             */
  #define SMPL_PSCL      0x41    /* 采样周期预分频   0~255   */
  #define SMPL_TDIV      0x42    /* 采样周期设置     0~65535 */
  #define SMPL_PSMP      0x43    /* 预采样深度设置   0~4095  */
  #define SMPL_RPTR      0x45    /* 数据读指针设置   0~4095  */ 
  #define SMPL_RD        0x46    /* 采样数据与状态读 9 bits  */
  #define SMPL_ST        0x47    /* FPGA 状态读出    6Bits   */

  //采样模式 SMPL_MODE 定义
  #define SEPARATE       0x00    /* ADC 并行采样 */
  #define INTERLACE      0x01    /* ADC 交错采样 */

  //触发控制参数设置命令
  #define TRIG_KIND      0x20    /* 触发类型设置      0~31     1 */
  #define TRIG_VOLT      0x21    /* 触发电平阈值      0~255  100 */
  #define TRIG_WDTH      0x22    /* 触发脉宽阈值     0~4095  35 */

  //测量数据读出选择定义
  #define VMIN           0x30    /* 模拟最小值 1B */
  #define VMAX           0x31    /* 模拟最大值 1B */
  #define VAVG           0x32    /* 模拟平均值 1B */
  #define ATWL           0x34    /* 模拟信号负脉宽*16      2B*1 */
  #define ATWH           0x35    /* 模拟信号正脉宽*16      2B*1 */
  #define DTWL           0x36    /* 数字信号负脉宽*16      2B*1 */
  #define DTWH           0x37    /* 数字信号正脉宽*16      2B*1 */

  //触发类型 TRIG_KIND 参数定义
  #define TRIG_NONE      0x00    /* 停止触发               */
  #define TRIG_ANY       0x01    /* 任意触发               */

  #define TRIG_AiDN      0x10    /* 模拟信号负阶跃触发     */
  #define TRIG_AiUP      0x11    /* 模拟信号正阶跃触发     */
  #define TRIG_AiLT      0x12    /* 模拟电平小于阈值触发   */
  #define TRIG_AiGT      0x13    /* 模拟电平大于阈值触发   */
  #define TRIG_AnLT      0x14    /* 模拟负脉宽小于阈值触发 */
  #define TRIG_AnGT      0x15    /* 模拟负脉宽大于阈值触发 */
  #define TRIG_ApLT      0x16    /* 模拟正脉宽小于阈值触发 */
  #define TRIG_ApGT      0x17    /* 模拟正脉宽大于阈值触发 */

  #define TRIG_DiDN      0x18    /* 数字信号负阶跃触发     */
  #define TRIG_DiUP      0x19    /* 数字信号正阶跃触发     */
  #define TRIG_DiLT      0x1A    /* 数字信号低电平触发     */
  #define TRIG_DiGT      0x1B    /* 数字信号高电平触发     */
  #define TRIG_DnLT      0x1C    /* 数字负脉宽小于阈值触发 */
  #define TRIG_DnGT      0x1D    /* 数字负脉宽大于阈值触发 */
  #define TRIG_DpLT      0x1E    /* 数字正脉宽小于阈值触发 */
  #define TRIG_DpGT      0x1F    /* 数字正脉宽大于阈值触发 */

#endif
