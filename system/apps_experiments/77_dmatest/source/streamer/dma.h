#define DMA_PERIPH_TO_MEMORY         ((uint32_t)0x00000000)      /*!< Peripheral to memory direction */
#define DMA_MEMORY_TO_PERIPH         ((uint32_t)0x00000010)     /*!< Memory to peripheral direction */
#define DMA_MEMORY_TO_MEMORY         ((uint32_t)0x00004000) /*!< Memory to memory direction     */
#define DMA_PINC_ENABLE              ((uint32_t)0x00000040)   /*!< Peripheral increment mode Enable */
#define DMA_PINC_DISABLE             ((uint32_t)0x00000000)     /*!< Peripheral increment mode Disable */
#define DMA_MINC_ENABLE              ((uint32_t)0x00000080)   /*!< Memory increment mode Enable  */
#define DMA_MINC_DISABLE             ((uint32_t)0x00000000)     /*!< Memory increment mode Disable */
#define DMA_PDATAALIGN_BYTE          ((uint32_t)0x00000000)        /*!< Peripheral data alignment: Byte     */
#define DMA_PDATAALIGN_HALFWORD      ((uint32_t)0x00000100)   /*!< Peripheral data alignment: HalfWord */
#define DMA_PDATAALIGN_WORD          ((uint32_t)0x00000200)   /*!< Peripheral data alignment: Word     */
#define DMA_MDATAALIGN_BYTE          ((uint32_t)0x00000000)        /*!< Memory data alignment: Byte     */
#define DMA_MDATAALIGN_HALFWORD      ((uint32_t)0x00000400)   /*!< Memory data alignment: HalfWord */
#define DMA_MDATAALIGN_WORD          ((uint32_t)0x00000800)   /*!< Memory data alignment: Word     */
#define DMA_NORMAL                   ((uint32_t)0x00000000)       /*!< Normal mode                  */
#define DMA_CIRCULAR                 ((uint32_t)0x00000020)     /*!< Circular mode                */
#define DMA_PRIORITY_LOW             ((uint32_t)0x00000000)     /*!< Priority level : Low       */
#define DMA_PRIORITY_MEDIUM          ((uint32_t)0x00001000)   /*!< Priority level : Medium    */
#define DMA_PRIORITY_HIGH            ((uint32_t)0x00002000)   /*!< Priority level : High      */
#define DMA_PRIORITY_VERY_HIGH       ((uint32_t)0x00003000)     /*!< Priority level : Very_High */

#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define __HAL_RCC_DMA1_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHBENR, RCC_AHBENR_DMA1EN);\
                                        /* Delay after an RCC peripheral clock enabling */\
                                        tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_DMA1EN);\
                                        ((void)(tmpreg)); \
                                      } while(0)

