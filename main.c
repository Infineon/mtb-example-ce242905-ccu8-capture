/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSOC™ Control C1 MCU: CCU8 capture Example
*              for ModusToolbox.
*
* Related Document: See README.md
*
******************************************************************************
* (c) 2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*****************************************************************************/
#include <stdio.h>
#include "cybsp.h"
#include "cy_utils.h"
#include "cy_retarget_io.h"

/*******************************************************************************
* Macros
*******************************************************************************/

#define CAPTURE_VALUE_EVENT0                    1
#define CAPTURE_VALUE_EVENT1                    3

/* Define macro to enable/disable printing of debug messages */
#define ENABLE_DEBUG_PRINT (0)

/* Define macro to set the loop count before printing debug messages */
#if ENABLE_DEBUG_PRINT
#define DEBUG_LOOP_COUNT_MAX                    2
#endif

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. It configures one of the CCU8 PWM slices to
* generate a PWM output signal, and another slice to capture the timer value
* at every rising edge and falling edge of the capture trigger signal. In this
* project PWM output from first slice is used as capture trigger for second
* slice.
*
* Output:
* This example project will print the capture value from second CCU8 slice at
* every rising and falling edge of the PWM signal.
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    uint16_t captureVal = 0;

    #if ENABLE_DEBUG_PRINT
    /* Assign false to disable printing of debug messages */
    static volatile bool debug_printf = true;

    /* Initialize the current loop count to zero */
    static uint32_t debug_loop_count = 0;
    #endif

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_HW);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    printf("\x1b[2J\x1b[;H");
    printf("**********************************\r\n");
    printf("CCU8 Capture Example Project\r\n");
    printf("**********************************\r\n");

    #if ENABLE_DEBUG_PRINT
    printf("Initialization done\r\n");
    #endif

    for (;;)
    {
        /* Continuously poll for Capture events 0 and 1 */
        if(Cy_CCU8_SLICE_GetEvent(CAPTURE_SLICE_HW, CY_CCU8_SLICE_IRQ_ID_EVENT0))
        {
            Cy_CCU8_SLICE_ClearEvent(CAPTURE_SLICE_HW, CY_CCU8_SLICE_IRQ_ID_EVENT0);
            captureVal = Cy_CCU8_SLICE_GetCaptureRegisterValue(CAPTURE_SLICE_HW, CAPTURE_VALUE_EVENT0);
            #if !(ENABLE_DEBUG_PRINT)
            printf("Rising edge:%d\r\n", captureVal);
            #endif
        }

        if(Cy_CCU8_SLICE_GetEvent(CAPTURE_SLICE_HW, CY_CCU8_SLICE_IRQ_ID_EVENT1))
        {
            Cy_CCU8_SLICE_ClearEvent(CAPTURE_SLICE_HW, CY_CCU8_SLICE_IRQ_ID_EVENT1);
            captureVal = Cy_CCU8_SLICE_GetCaptureRegisterValue(CAPTURE_SLICE_HW, CAPTURE_VALUE_EVENT1);
            #if !(ENABLE_DEBUG_PRINT)
            printf("Falling edge:%d\r\n", captureVal);
            #endif

            #if ENABLE_DEBUG_PRINT
            debug_loop_count++;

            if(debug_printf && debug_loop_count == DEBUG_LOOP_COUNT_MAX )
            {
                debug_printf = false;

                /* Print message after the loop has run after DEBUG_LOOP_COUNT_MAX times */
                printf("Capture values printed\r\n");
            }
            #endif
        }
    }
}

/* [] END OF FILE */
