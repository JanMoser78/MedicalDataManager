/**
 * This software is copyrighted by Bosch Connected Devices and Solutions GmbH, 2016.
 * The use of this software is subject to the XDK SDK EULA
 */
//lint -esym(956,*) /* Suppressing "Non const, non volatile static or external variable" lint warning*/

/* module includes ********************************************************** */

/* system header files */
#include <stdio.h>
/* additional interface header files */
#include "simplelink.h"
#include "BCDS_Basics.h"
#include "BCDS_Assert.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "BCDS_WlanConnect.h"
#include "BCDS_NetworkConfig.h"
#include "button.h"
#include "XdkBoardHandle.h"
#include "XdkSensorHandle.h"
#include <Serval_Types.h>
#include <Serval_Basics.h>
#include <Serval_Ip.h>
/* own header files */
#include "SendDataOverUdp.h"
#include <BCDS_Environmental.h>
#include <BCDS_SensorErrorType.h>

/* constant definitions ***************************************************** */

/* local variables ********************************************************** */

/**
 * This buffer holds the data to be sent to server via UDP
 * */
static uint16_t bsdBuffer_mau[BUFFER_SIZE] = { (uint16_t) ZERO };
/**
 * Timer handle for connecting to wifi and obtaining the IP address
 */
xTimerHandle wifiConnectTimerHandle_gdt = NULL;
/**
 * Timer handle for periodically sending data over wifi
 */
xTimerHandle wifiSendTimerHandle = NULL;

/* global variables ********************************************************* */
static BUTTON_handle_tp Button1Handle = (BUTTON_handle_tp) NULL; /**< variable to store button 1 handle */
static BUTTON_handle_tp Button2Handle = (BUTTON_handle_tp) NULL; /**< variable to store button 2 handle */

/* inline functions ********************************************************* */

/* local functions ********************************************************** */
static returnTypes_t sendTokenViaUdp(int token);
return_t ledAndButtonInit(void);

/**
 *  @brief
 *      Function to initialize the wifi network send application. Create timer task
 *      to start WiFi Connect and get IP function after one second. After that another timer
 *      to send data periodically.
 */
void init(void)
{
    uint32_t Ticks = UINT32_C(1000);

    if (Ticks != UINT32_MAX) /* Validated for portMAX_DELAY to assist the task to wait Infinitely (without timing out) */
    {
        Ticks /= portTICK_RATE_MS;
    }
    if (UINT32_C(0) == Ticks) /* ticks cannot be 0 in FreeRTOS timer. So ticks is assigned to 1 */
    {
        Ticks = UINT32_C(1);
    }
    /* create timer task*/
    wifiConnectTimerHandle_gdt = xTimerCreate((char * const ) "wifiConnect", Ticks, TIMER_AUTORELOAD_OFF, NULL, wifiConnectGetIP);
    wifiSendTimerHandle = xTimerCreate((char * const ) "wifiSend", Ticks, TIMER_AUTORELOAD_ON, NULL, wifiSend);

    if ((wifiConnectTimerHandle_gdt != NULL) && (wifiSendTimerHandle != NULL))
    {
        /*start the wifi connect timer*/
        if ( xTimerStart(wifiConnectTimerHandle_gdt, TIMERBLOCKTIME) != pdTRUE)
        {
            assert(false);
        }
    }
    else
    {
        /* Assertion Reason: "Failed to create timer task during initialization"   */
        assert(false);
    }

}

/**
 * @brief This is a template function where the user can write his custom application.
 *
 */
void appInitSystem(xTimerHandle xTimer)
{
    BCDS_UNUSED(xTimer);
    /*Call the WNS module init API */
    init();
}

/**
 * @brief Opening a UDP client side socket and sending data on a server port
 *
 * This function opens a UDP socket and tries to connect to a Server SERVER_IP
 * waiting on port SERVER_PORT.
 * Then the function will send periodic UDP packets to the server.
 * 
 * @param[in] port
 *					destination port number
 *
 * @return         returnTypes_t:
 *                                  SOCKET_ERROR: when socket has not opened properly
 *                                  SEND_ERROR: when 0 transmitted bytes or send error
 *                                  STATUS_OK: when UDP sending was successful
 */

static returnTypes_t bsdUdpClient(uint16_t port)
{
	Environmental_Data_T  sensorData;
	Environmental_readData( xdkEnvironmental_BME280_Handle,  &sensorData); //humidity, pressure, temperature

    static uint16_t counter = UINT16_C(0);
    SlSockAddrIn_t Addr;
    uint16_t AddrSize = (uint16_t) ZERO;
    int16_t SockID = (int16_t) ZERO;
    int16_t Status = (int16_t) ZERO;

    /* copies the dummy data to send array , the first array element is the running counter to track the number of packets send so far*/
    bsdBuffer_mau[1] = sensorData.humidity;
    bsdBuffer_mau[2] = sensorData.pressure;
    bsdBuffer_mau[3] = sensorData.temperature;
    bsdBuffer_mau[0] = 0xCC04;
    Addr.sin_family = SL_AF_INET;
    Addr.sin_port = sl_Htons((uint16_t) port);
    Addr.sin_addr.s_addr = sl_Htonl(SERVER_IP);
    AddrSize = sizeof(SlSockAddrIn_t);

    SockID = sl_Socket(SL_AF_INET, SL_SOCK_DGRAM, (uint32_t) ZERO); /**<The return value is a positive number if successful; other wise negative*/
    if (SockID < (int16_t) ZERO)
    {
        /* error case*/
        return (SOCKET_ERROR);
    }

    Status = sl_SendTo(SockID, bsdBuffer_mau, BUFFER_SIZE * sizeof(uint16_t), (uint32_t) ZERO, (SlSockAddr_t *) &Addr, AddrSize);/**<The return value is a number of characters sent;negative if not successful*/
    /*Check if 0 transmitted bytes sent or error condition*/
    if (Status <= (int16_t) ZERO)
    {
        Status = sl_Close(SockID);
        if (Status < 0)
        {
            return (SEND_ERROR);
        }
        return (SEND_ERROR);
    }
    Status = sl_Close(SockID);
    if (Status < 0)
    {
        return (SEND_ERROR);
    }
    counter++;
    return (STATUS_OK);
}
/**
 *  @brief
 *      Function to periodically send data over WiFi as UDP packets. This is run as an Auto-reloading timer.
 *
 *  @param [in ] xTimer - necessary parameter for timer prototype
 */
static void wifiSend(xTimerHandle xTimer)
{
    BCDS_UNUSED(xTimer);
    if (STATUS_OK != bsdUdpClient(SERVER_PORT))
    {
        /* assertion Reason:  "Failed to  send udp packet" */
        assert(false);
        printf("ERROR\n\r");
    }
}

/**
 *  @brief
 *      Function to connect to wifi network and obtain IP address
 *
 *  @param [in ] xTimer
 */
static void wifiConnectGetIP(xTimerHandle xTimer)
{
    BCDS_UNUSED(xTimer);

    int error;

    error = BCDS_getSensorErrorCode(Environmental_init  ( xdkEnvironmental_BME280_Handle ));
    printf("Init exit with status %d \n\r", error);

    error = BCDS_getSensorErrorCode(Environmental_setPowerMode (xdkEnvironmental_BME280_Handle, ENVIRONMENTAL_BME280_POWERMODE_NORMAL));
    printf("Setting up power mode exit with status %d\n\r", error);


	if (RETCODE_OK == Environmental_setOverSamplingPressure(xdkEnvironmental_BME280_Handle, ENVIRONMENTAL_BME280_OVERSAMP_8X))
	{
	  printf("Environmental_setOverSamplingPressure Success\n\r");
	}
	else
		printf("Environmental_setOverSamplingPressure Failed\n\r");

	if (RETCODE_OK == Environmental_setOverSamplingHumidity(xdkEnvironmental_BME280_Handle, ENVIRONMENTAL_BME280_OVERSAMP_8X))
	{
	   printf("Environmental_setOverSamplingHumidity Success\n\r");
	}
	else
		printf("Environmental_setOverSamplingHumidity Failed\n\r");

	 if (RETCODE_OK == Environmental_setOverSamplingTemperature(xdkEnvironmental_BME280_Handle, ENVIRONMENTAL_BME280_OVERSAMP_8X))
	{
	   printf("Environmental_setOverSamplingTemperature Success\n\r");
	}
	else
		 printf("Environmental_setOverSamplingTemperature Failed\n\r");

	ledAndButtonInit();

    NCI_ipSettings_t myIpSettings;
    memset(&myIpSettings, (uint32_t) 0, sizeof(myIpSettings));
    char ipAddress[PAL_IP_ADDRESS_SIZE] = { 0 };
    Ip_Address_T* IpaddressHex = Ip_getMyIpAddr();
    WLI_connectSSID_t connectSSID;
    WLI_connectPassPhrase_t connectPassPhrase;
    NCI_return_t ReturnValue = NCI_FAILURE;
    int32_t Result = INT32_C(-1);

    if (WLI_SUCCESS != WLI_init())
    {
        printf("Error occurred initializing WLAN \r\n ");
        return;
    }

    printf("Connecting to %s \r\n ", WLAN_CONNECT_WPA_SSID);

    connectSSID = (WLI_connectSSID_t) WLAN_CONNECT_WPA_SSID;
    connectPassPhrase = (WLI_connectPassPhrase_t) WLAN_CONNECT_WPA_PASS;
    ReturnValue = NCI_setIpDhcp(NULL);
    if (ReturnValue != NCI_SUCCESS)
    {
        printf("Error in setting IP to DHCP\n\r");
        return;
    }

    if (WLI_SUCCESS == WLI_connectWPA(connectSSID, connectPassPhrase, NULL))
    {
        ReturnValue = NCI_getIpSettings(&myIpSettings);
        if (NCI_SUCCESS == ReturnValue)
        {
            *IpaddressHex = Basics_htonl(myIpSettings.ipV4);
            Result = Ip_convertAddrToString(IpaddressHex, ipAddress);
            if (Result < 0)
            {
                printf("Couldn't convert the IP address to string format \r\n ");
                return;
            }
            printf("Connected to WPA network successfully. \r\n ");
            printf(" Ip address of the device: %s \r\n ", ipAddress);
        }
        else
        {
            printf("Error in getting IP settings\n\r");
            return;
        }
    }
    else
    {
        printf("Error occurred connecting %s \r\n ", WLAN_CONNECT_WPA_SSID);
        return;
    }

    /* After connection start the wifi sending timer*/
    if (xTimerStart(wifiSendTimerHandle, TIMERBLOCKTIME) != pdTRUE)
    {
        assert(false);
    }
}

/* global functions ********************************************************* */

/** ************************************************************************* */
return_t ledAndButtonInit(void)
{
    return_t returnValue = FAILURE;

    returnValue = createButton();
    if (returnValue == SUCCESS)
    {
        returnValue = enableButton();
    }
    else
    {
        printf("Error in creating button\n\r");

    }
    if (returnValue == SUCCESS)
    {
        returnValue = setButtonCallback();
    }
    else
    {
        printf("Error in enabling button\n\r");
    }
    return (returnValue);
}

static return_t createButton(void)
{
    return_t returnValue = FAILURE;

    Button1Handle = BUTTON_create(gpioButton1_Handle, GPIO_STATE_OFF);
    if (Button1Handle != NULL)
    {
        Button2Handle = BUTTON_create(gpioButton2_Handle, GPIO_STATE_OFF);
    }
    if (Button2Handle != NULL)
    {
        returnValue = SUCCESS;
    }
    return (returnValue);
}

static return_t enableButton(void)
{
    return_t returnValue = FAILURE;
    BUTTON_errorTypes_t buttonReturn = BUTTON_ERROR_INVALID_PARAMETER;
    buttonReturn = BUTTON_enable(Button1Handle);
    if (buttonReturn == BUTTON_ERROR_OK)
    {
        buttonReturn = BUTTON_enable(Button2Handle);
    }
    if (buttonReturn == BUTTON_ERROR_OK)
    {
        returnValue = SUCCESS;
    }
    return (returnValue);
}

static return_t setButtonCallback(void)
{
    return_t returnValue = FAILURE;
    BUTTON_errorTypes_t buttonReturn = BUTTON_ERROR_INVALID_PARAMETER;

    buttonReturn = BUTTON_setCallback(Button1Handle, callback, CALLBACK_PARAMETER_PB1);
    if (buttonReturn == BUTTON_ERROR_OK)
    {
        buttonReturn = BUTTON_setCallback(Button2Handle, callback, CALLBACK_PARAMETER_PB2);
    }
    if (buttonReturn == BUTTON_ERROR_OK)
    {
        returnValue = SUCCESS;
    }
    return (returnValue);
}
static void callback(void *handle, uint32_t userParameter)
{
    switch (userParameter)
    {
    /*  Button 1 press/release */
    case CALLBACK_PARAMETER_PB1:
        if (BUTTON_isPressed(handle))
        {
        	sendTokenViaUdp(20);
        }
        if (BUTTON_isReleased(handle))
        {

        }
        break;

        /* Button 2 press/release */
    case CALLBACK_PARAMETER_PB2:
        if (BUTTON_isPressed(handle))
        {
        	sendTokenViaUdp(10);
        }
        if (BUTTON_isReleased(handle))
        {

        }
        break;
    default:
        printf("Button not available \n\r");
        break;
    }
}

static returnTypes_t sendTokenViaUdp(int token)
{
    SlSockAddrIn_t Addr;
    uint16_t AddrSize = (uint16_t) ZERO;
    int16_t SockID = (int16_t) ZERO;
    int16_t Status = (int16_t) ZERO;

    /* copies the dummy data to send array , the first array element is the running counter to track the number of packets send so far*/
    bsdBuffer_mau[0] = token;
    Addr.sin_family = SL_AF_INET;
    Addr.sin_port = sl_Htons((uint16_t) 6665);
    Addr.sin_addr.s_addr = sl_Htonl(SERVER_IP);
    AddrSize = sizeof(SlSockAddrIn_t);

    SockID = sl_Socket(SL_AF_INET, SL_SOCK_DGRAM, (uint32_t) ZERO); /**<The return value is a positive number if successful; other wise negative*/
    if (SockID < (int16_t) ZERO)
    {
        /* error case*/
        return (SOCKET_ERROR);
    }

    Status = sl_SendTo(SockID, bsdBuffer_mau, sizeof(uint16_t), (uint32_t) ZERO, (SlSockAddr_t *) &Addr, AddrSize);/**<The return value is a number of characters sent;negative if not successful*/
    /*Check if 0 transmitted bytes sent or error condition*/
    if (Status <= (int16_t) ZERO)
    {
        Status = sl_Close(SockID);
        if (Status < 0)
        {
            return (SEND_ERROR);
        }
        return (SEND_ERROR);
    }
    Status = sl_Close(SockID);
    if (Status < 0)
    {
        return (SEND_ERROR);
    }

    printf("Token %d correctly send !", token);
    return (STATUS_OK);
}
