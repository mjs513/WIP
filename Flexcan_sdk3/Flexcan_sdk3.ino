#include "fsl_flexcan.h"

#define EXAMPLE_CAN CAN2
#define EXAMPLE_FLEXCAN_IRQn CAN2_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler CAN2_IRQHandler

/*******************************************************************************
 * Variables
 ******************************************************************************/
flexcan_handle_t flexcanHandle;

volatile bool txComplete = false;
volatile bool rxComplete = false;
flexcan_mb_transfer_t txXfer, rxXfer;
flexcan_frame_t txFrame, rxFrame;
uint32_t txIdentifier;
uint32_t rxIdentifier;
#define RX_MESSAGE_BUFFER_NUM (9)
#define TX_MESSAGE_BUFFER_NUM (8)
#define EXAMPLE_CAN_CLK_FREQ (20000000)
    uint8_t node_type;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief FlexCAN Call Back function
 */
static void EXAMPLE_FLEXCAN_IRQHandler(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
    /* If new data arrived. */
    if (FLEXCAN_GetMbStatusFlags(EXAMPLE_CAN, 1 << RX_MESSAGE_BUFFER_NUM))
    {
        FLEXCAN_ClearMbStatusFlags(EXAMPLE_CAN, 1 << RX_MESSAGE_BUFFER_NUM);
        FLEXCAN_ReadRxMb(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &rxFrame);
        rxComplete = true;
    }

}



#define PRINTF Serial2.printf
/*!
 * @brief Main function
 */
void setup(void)
{
    Serial2.begin(1843200);
    EnableIRQ(EXAMPLE_FLEXCAN_IRQn);
      
    flexcan_config_t flexcanConfig;
    flexcan_rx_mb_config_t mbConfig;

  //clock settings for flexcan
  CCM_CSCMR2 = (CCM_CSCMR2 & 0xFFFFFC03) | CCM_CSCMR2_CAN_CLK_SEL(2) |CCM_CSCMR2_CAN_CLK_PODF(3);
  Serial2.print("CCM_CSCMR2: ");Serial2.println(CCM_CSCMR2, BIN);
  
  //CCM_CCGR0 |= (0xF << 18);
  CCM_CCGR0 |= CCM_CCGR0_CAN2(CCM_CCGR_ON); 
  CCM_CCGR0 |= CCM_CCGR0_CAN2_SERIAL(CCM_CCGR_ON);
  Serial2.print("CCM_CCGR0: ");Serial2.println(CCM_CCGR0, BIN);

    //-------------------------------------//
    //rx configure pin for flexcan
    CORE_PIN0_CONFIG = 0x10;        // alt 0 is can2
    IOMUXC_FLEXCAN2_RX_SELECT_INPUT = 0x01;
    Serial2.print("IOMUXC_FLEXCAN2_RX_SELECT_INPUT: ");Serial2.println(IOMUXC_FLEXCAN2_RX_SELECT_INPUT, BIN);
  
    //pin1, can2TX B0_02
    CORE_PIN1_CONFIG = 0x10;
    uint32_t fastio =  IOMUXC_PAD_SRE | IOMUXC_PAD_DSE(3) | IOMUXC_PAD_SPEED(3);
    CORE_PIN0_PADCONFIG = 0xB0;
    CORE_PIN1_PADCONFIG = 0xB0;  // 0x1A081, 0x10B0


  
    PRINTF("********* FLEXCAN Interrupt EXAMPLE *********\r\n");
    PRINTF("    Message format: Standard (11 bit id)\r\n");
    PRINTF("    Message buffer %d used for Rx.\r\n", RX_MESSAGE_BUFFER_NUM);
    PRINTF("    Message buffer %d used for Tx.\r\n", TX_MESSAGE_BUFFER_NUM);
    PRINTF("    Interrupt Mode: Enabled\r\n");
    PRINTF("    Operation Mode: TX and RX --> Normal\r\n");
    PRINTF("*********************************************\r\n\r\n");

    node_type = 'A';
    /* Select mailbox ID. */
    if ((node_type == 'A') || (node_type == 'a'))
    {
        txIdentifier = 0x321;
        rxIdentifier = 0x123;
    }
    else
    {
        txIdentifier = 0x123;
        rxIdentifier = 0x321;
    }

    /* Get FlexCAN module default Configuration. */
    /*
     * flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
     * flexcanConfig.baudRate = 1000000U;
     * flexcanConfig.maxMbNum = 16;
     * flexcanConfig.enableLoopBack = false;
     * flexcanConfig.enableSelfWakeup = false;
     * flexcanConfig.enableIndividMask = false;
     * flexcanConfig.enableDoze = false;
     * flexcanConfig.timingConfig = timingConfig;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);

    /* Init FlexCAN module. */

    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);

    PRINTF("\nFLEXCAN2_MCR = %x\n", FLEXCAN2_MCR);
    PRINTF("\nFLEXCAN2_CTRL1 = %x\n", FLEXCAN2_CTRL1);
    PRINTF("\nFLEXCAN2_TIMER = %x\n", FLEXCAN2_TIMER);
    PRINTF("\nFLEXCAN2_IMASK1 = %x\n", FLEXCAN2_IMASK1);
    PRINTF("\nFLEXCAN2_IFLAG2 = %x\n", FLEXCAN2_IFLAG2);
    PRINTF("\nFLEXCAN2_IFLAG1 = %x\n", FLEXCAN2_IFLAG1);
    PRINTF("\nFLEXCAN2_CTRL2 = %x\n", FLEXCAN2_CTRL2);
    PRINTF("\nFLEXCAN2_CTRL1 = %x\n", FLEXCAN2_MCR);
    PRINTF("\nFLEXCAN2_ESR2 = %x\n", FLEXCAN2_ESR2);
    PRINTF("\nFLEXCAN2_CRCR = %x\n", FLEXCAN2_CRCR);
    PRINTF("\nFLEXCAN2_GFWR = %x\n", FLEXCAN2_GFWR);   

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, EXAMPLE_FLEXCAN_IRQHandler, NULL);

    /* Set Rx Masking mechanism. */
    FLEXCAN_SetRxMbGlobalMask(EXAMPLE_CAN, FLEXCAN_RX_MB_STD_MASK(rxIdentifier, 0, 0));

    /* Setup Rx Message Buffer. */
    mbConfig.format = kFLEXCAN_FrameFormatStandard;
    mbConfig.type = kFLEXCAN_FrameTypeData;
    mbConfig.id = FLEXCAN_ID_STD(rxIdentifier);
    FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig, true);

    /* Setup Tx Message Buffer. */

    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

    if ((node_type == 'A') || (node_type == 'a'))
    {
        PRINTF("Press any key to trigger one-shot transmission\r\n\r\n");
        //frame.dataByte0 = 0;
    }
    else
    {
        PRINTF("Start to Wait data from Node A\r\n\r\n");
    }

}

void loop(){
            txFrame.id = FLEXCAN_ID_STD(txIdentifier);
            txFrame.format = kFLEXCAN_FrameFormatStandard;
            txFrame.type = kFLEXCAN_FrameTypeData;
            txFrame.length = 8;
            txFrame.dataByte0 = 0;
            txFrame.dataByte1 = 1;
            txFrame.dataByte2 = 2;
            txFrame.dataByte3 = 3;
            txFrame.dataByte4 = 4;
            txFrame.dataByte5 = 1;
            txFrame.dataByte6 = 2;
            txFrame.dataByte7 = 3;
            //txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;

            //txXfer.frame = &frame;
            FLEXCAN_TransferSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);
  
            /* Start receive data through Rx Message Buffer. */
            FLEXCAN_TransferReceiveBlocking(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &rxFrame);

        /* Wait until Rx receive full. */
        /*    while (!rxComplete)
            {
              PRINTF("Waiting for rxComplete!\r\n\r\n");
            };
            rxComplete = false;
       */
            PRINTF("Rx MB ID: 0x%3x, Rx MB data: 0x%x\r\n", rxFrame.id >> CAN_ID_STD_SHIFT, rxFrame.dataByte0);
            PRINTF("Rx MB data: 0x%x  %x  %x  %x  %x  %x  %x  %x\r\n",rxFrame.dataByte0,rxFrame.dataByte1,rxFrame.dataByte2,
                rxFrame.dataByte3,rxFrame.dataByte4,rxFrame.dataByte4,rxFrame.dataByte6,rxFrame.dataByte7);


    }
