/******************** (C) COPYRIGHT 2018 e-Design Co., Ltd. ********************
 Ident.h Ver 2.0                                        Author : bure
********************************************************************************

 * 调用时需要外部提供的硬件相关宏定义如下：

   #define TYPE_DS201 / TYPE_DS203 / ...   产品型号
   #define LIC_REC      0x08xxxxxx         Licence 存放地址

*******************************************************************************/
#ifndef __IDENT_H
#define __IDENT_H

  #include "Platform.h"

  #define TYPE_LA104    // 暂时借用 LA104 的 Lic 解锁识别号
//  #define TYPE_DS213

  #define LIC_REC        0x080000FC

 // 采用统一名称可供外部调用的函数如下：

  u32 Dev_SN(u32 Base);
  u32 AnswerCode(void);
  u8  CheckLic(u32 SN_Base);
  u8  CompareLic(u32 Lic_No, u32 SN_Base);
  u32 UserLicGen(u32 User_ID, u32 SN_Base, u32 Proj_ID);

#endif

/********************************* END OF FILE ********************************/
