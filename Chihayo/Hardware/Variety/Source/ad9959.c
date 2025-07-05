#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__AD9959_ENABLE__	// begin of __HARDWARE_CONFIG__AD9959_ENABLE__
//*******************************// include _h files    //************************************//
#include "ad9959.h"
//*******************************// define parameters   //************************************//
#define AD9959_LED
//*******************************// parameters          //************************************//

typedef struct
{
    GPIO_TypeDef *GPIOx;
    uint32_t Pin;
} driverIO;

// Forced IO definitions
driverIO SDIO0 = 	{AD9959_SD0_GPIO_Port, 	AD9959_SD0_Pin};
driverIO SDIO1 = 	{AD9959_SD1_GPIO_Port, 	AD9959_SD1_Pin};
driverIO SDIO2 = 	{AD9959_SD2_GPIO_Port, 	AD9959_SD2_Pin};
driverIO SDIO3 = 	{AD9959_SD3_GPIO_Port, 	AD9959_SD3_Pin};
driverIO PDC = 		{AD9959_PDC_GPIO_Port, 	AD9959_PDC_Pin};
driverIO RST = 		{AD9959_RST_GPIO_Port, 	AD9959_RST_Pin};
driverIO SCLK = 	{AD9959_SCK_GPIO_Port, 	AD9959_SCK_Pin};
driverIO CS = 		{AD9959_CSB_GPIO_Port, 	AD9959_CSB_Pin};
driverIO UPDATE = 	{AD9959_IUP_GPIO_Port, 	AD9959_IUP_Pin};
driverIO PS0 = 		{AD9959_P0_GPIO_Port, 	AD9959_P0_Pin};
driverIO PS1 = 		{AD9959_P1_GPIO_Port, 	AD9959_P1_Pin};
driverIO PS2 = 		{AD9959_P2_GPIO_Port, 	AD9959_P2_Pin};
driverIO PS3 = 		{AD9959_P3_GPIO_Port, 	AD9959_P3_Pin};

#ifdef AD9959_LED
// Optional IO define

#endif

uint8_t CSR_DATA0[1] = {0x10}; // Enable CH0
uint8_t CSR_DATA1[1] = {0x20}; // Enable CH1
uint8_t CSR_DATA2[1] = {0x40}; // Enable CH2
uint8_t CSR_DATA3[1] = {0x80}; // Enable CH3

uint8_t CSR_DATA[4] = {0x10, 0x20, 0x40, 0x80}; // Enable CH0,CH1,CH2,CH3

uint8_t FR2_DATA[2] = {0x00, 0x00}; // default Value = 0x0000

uint8_t CPOW0_DATA[2] = {0x00, 0x00}; // default Value = 0x0000   @ = POW/2^14*360

uint32_t SinFre[4] = {100000000, 100000000, 100000000, 100000000};
uint32_t SinAmp[4] = {1023, 1023, 1023, 1023};
uint32_t SinPhr[4] = {0, 4095, 4095 * 3, 4095 * 2};

//*******************************// define function     //************************************//

/**
 * @brief Initializes the AD9959 module.
 */
void Init_AD9959(void)
{
    // GPIO need to be initialized before calling this function

    uint8_t FR1_DATA[3] = {0xD3, 0x00, 0x00}; // 20 frequency doubling
    uint8_t CFR_DATA[3] = {0x00, 0x03, 0x00}; // default Value = 0x000302
    InitIO_9959();
    InitReset();

    WriteData_AD9959(FR1_ADD, 3, FR1_DATA, 1);
    WriteData_AD9959(CFR_ADD, 3, CFR_DATA, 1);

    Write_Phase(3, SinPhr[3]);
    Write_Phase(0, SinPhr[0]);
    Write_Phase(1, SinPhr[1]);
    Write_Phase(2, SinPhr[2]);

    Write_frequency(0, SinFre[0]);
    Write_frequency(1, SinFre[1]);
    Write_frequency(2, SinFre[2]);
    Write_frequency(3, SinFre[3]);

    Write_Amplitude(3, SinAmp[3]);
    Write_Amplitude(0, SinAmp[0]);
    Write_Amplitude(1, SinAmp[1]);
    Write_Amplitude(2, SinAmp[2]);
}

/**
 * @brief Delays the execution for a specified amount of time.
 *
 * @param length The length of the delay in loop.
 */
void delay_9959(uint32_t length)
{
    length = length * 12;
    while (length--)
        ;
}

/**
 * @brief Initializes the IO for AD9959.
 */
void InitIO_9959(void)
{

    WRT(PDC, 0);
    WRT(CS, 1);
    WRT(SCLK, 0);
    WRT(UPDATE, 0);
    WRT(PS0, 0);
    WRT(PS1, 0);
    WRT(PS2, 0);
    WRT(PS3, 0);
    WRT(SDIO0, 0);
    WRT(SDIO1, 0);
    WRT(SDIO2, 0);
    WRT(SDIO3, 0);
}

void InitReset(void)
{
    WRT(RST, 0);
    delay_9959(1);
    WRT(RST, 1);
    delay_9959(30);
    WRT(RST, 0);
}

/**
 * @brief Updates the IO state of the AD9959 device.
 *
 * This function is responsible for updating the IO state of the AD9959 device.
 * It triggers the IO update operation to ensure that any pending changes in the
 * control registers are applied to the device.
 */
void IO_Update(void)
{
    WRT(UPDATE, 1);
    delay_9959(1);
    WRT(UPDATE, 0);
}

/**
 * @brief Writes data to the AD9959 device.
 *
 * This function writes data to the specified register address of the AD9959 device.
 *
 * @param RegisterAddress The address of the register to write data to.
 * @param NumberofRegisters The number of registers to write data to.
 * @param RegisterData Pointer to the data to be written to the registers.
 * @param update Flag indicating whether to update the device after writing data.
 *               Set to 1 to update the device, or 0 to skip the update.
 */
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData, uint8_t update)
{
    uint8_t ControlValue = 0;
    uint8_t ValueToWrite = 0;
    uint8_t RegisterIndex = 0;
    uint8_t i = 0;

    ControlValue = RegisterAddress;
    // Write address
    WRT(SCLK, 0);
    WRT(CS, 0);
    for (i = 0; i < 8; i++)
    {
        WRT(SCLK, 0);
        if (0x80 == (ControlValue & 0x80))
            WRT(SDIO0, 1);
        else
            WRT(SDIO0, 0);
        WRT(SCLK, 1);
        ControlValue <<= 1;
        delay_9959(2);
    }

    WRT(SCLK, 0);
    // Write data
    // ÿ���ֽڵ����ݶ��ǴӸ�λ��ʼ����
    // ��һ��������ֽ������λ���ֽ�
    for (RegisterIndex = 0; RegisterIndex < NumberofRegisters; RegisterIndex++)
    {
        ValueToWrite = RegisterData[RegisterIndex];
        for (i = 0; i < 8; i++)
        {
            WRT(SCLK, 0);
            if (0x80 == (ValueToWrite & 0x80))
                WRT(SDIO0, 1);
            else
                WRT(SDIO0, 0);
            WRT(SCLK, 1);
            ValueToWrite <<= 1;
            delay_9959(2);
        }
        WRT(SCLK, 0);
    }
    if (update == 1)
        IO_Update();
    WRT(CS, 1);
}

/**
 * @brief Writes the frequency value to the specified channel.
 *
 * @param Channel The channel number to write the frequency to.(0 to 3)
 * @param Freq The frequency value to be written.(1 to 500000000)
 */
void Write_frequency(uint8_t Channel, uint32_t Freq)
{
    if (Freq > 500000000 || Freq < 1)
    {
        Freq = 114514;
        AD9959_error();
    }

    uint8_t CFTW0_DATA[4] = {0x00, 0x00, 0x00, 0x00};
    uint32_t Temp;
    Temp = (uint32_t)(20 * Freq) * 8.589934592; // The input frequency factor is divided into four bytes.  4.294967296=(2^32)/500000000
    CFTW0_DATA[3] = (uint8_t)Temp;
    CFTW0_DATA[2] = (uint8_t)(Temp >> 8);
    CFTW0_DATA[1] = (uint8_t)(Temp >> 16);
    CFTW0_DATA[0] = (uint8_t)(Temp >> 24);
    Channel_Select(Channel);
    WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1); // CTW0 address 0x04.Output CH0 setting frequency
}

/**
 * @brief Writes the amplitude value for a specific channel.
 *
 * @param Channel The channel number.(0 to 3)
 * @param Ampli The amplitude value to be written.(0 to 1023)
 */
void Write_Amplitude(uint8_t Channel, uint16_t Ampl)
{
    // Ampli��ȡֵ��0��1023֮��
    if ((Ampl << 1) > 1023)
    {
        Ampl = 114;
        AD9959_error();
    }

    uint16_t A_temp;                          //=0x23ff;
    uint8_t ACR_DATA[3] = {0x00, 0x00, 0x00}; // default Value = 0x--0000 Rest = 18.91/Iout

    A_temp = (Ampl << 1) | 0x1000;
    ACR_DATA[2] = (uint8_t)A_temp;        // Low bit data
    ACR_DATA[1] = (uint8_t)(A_temp >> 8); // High bit data
    Channel_Select(Channel);
    WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
}

/**
 * @brief  Set up the Phase
 * @name  Write_Phase
 * @example Phase=4095 => Degree=pi/2||Phase=9215 => Degree=pi||Phase=16383 => Degree=2pi
 * @param  channel, Phase
 * @retval None
 */
void Write_Phase(uint8_t Channel, uint16_t Phase)
{
    // Phase_max = 16383
    if (Phase > 16383)
    {
        Phase = 16383;
        AD9959_error();
    }
    uint16_t P_temp = 0;
    P_temp = (uint16_t)Phase;
    CPOW0_DATA[1] = (uint8_t)P_temp;
    CPOW0_DATA[0] = (uint8_t)(P_temp >> 8);
    Channel_Select(Channel);
    WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 1);
}

/**
 * @brief  Select the channel
 * @name  Channel_Select
 * @param  channel
 * @retval None
 */
void Channel_Select(uint8_t Channel)
{
    if (Channel > 3)
    {
        AD9959_error();
        return; 
    }
    else
        WriteData_AD9959(CSR_ADD, 1, CSR_DATA + Channel, 0);
}

/**
 * @brief  AD9959 Error handler
 * CAN BE MODIFIED
 * @note   LED0 and LED1 will blink 10 times
 * @name  AD9959_error
 * @param  None
 * @retval None
 */
void AD9959_error(void)
{
#ifdef AD9959_LED
    int i = 10;
    while (i > 0)
    {
        //HAL_GPIO_TogglePin(AD9959_LED_GPIO_Port,AD9959_LED_Pin);

        HAL_Delay(500);
        //HAL_GPIO_TogglePin(AD9959_LED_GPIO_Port,AD9959_LED_Pin);

        i--;
    }
    //HAL_GPIO_TogglePin(AD9959_LED_GPIO_Port,AD9959_LED_Pin);

#endif
}

/**
 * @brief Performs a frequency sweep on the AD9959 device.
 *
 * @param Channel The channel number to perform the sweep on.
 * @param Start_Freq The starting frequency of the sweep.
 * @param Stop_Freq The ending frequency of the sweep.
 * @param Step The frequency step size for each iteration of the sweep.
 * @param time The duration of each frequency step in microsecond.(1-2048)j
 * @param NO_DWELL
 */
void Sweep_Frequency(uint8_t Channel, uint32_t Start_Freq, uint32_t Stop_Freq, uint32_t Step, uint32_t time, uint8_t NO_DWELL)
{
    Channel_Select(Channel);
    switch (Channel)
    {
    case 0:
        WRT(PS0, 0);
    case 1:
        WRT(PS1, 0);
    case 2:
        WRT(PS2, 0);
    case 3:
        WRT(PS3, 0);
    }

    uint8_t CFR_GET[3] = {0x00, 0x00, 0x00};
    ReadData_AD9959(CFR_ADD, 3, CFR_GET);
    uint8_t CFR_DATA_Freq[3] = {CFR_GET[0] | 0x80, CFR_GET[1] | 0x40, CFR_GET[2] | 0x00};
    // Disable no-dwell bit by writing 0 to CFR[13]
    if (NO_DWELL)
        CFR_DATA_Freq[1] = CFR_DATA_Freq[1] | 0x80;

    uint8_t FR1_DATA_sweep[3] = {0xD3, 0x00, 0x00};
    WriteData_AD9959(CFR_ADD, 3, CFR_DATA_Freq, 0);
    WriteData_AD9959(FR1_ADD, 3, FR1_DATA_sweep, 0);

    uint32_t Start_Freq_word, Stop_Freq_Word, Step_Word;
    uint8_t Time_word;
    Start_Freq_word = (uint32_t)Start_Freq * 8.589934592;
    Stop_Freq_Word = (uint32_t)Stop_Freq * 8.589934592;
    Step_Word = (uint32_t)Step * 8.589934592;
    Time_word = (uint8_t)time / 8.0; // ns

    uint8_t CFTW0_DATA_START[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t CFTW0_DATA_STOP[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t RDW_DATA[4] = {0x00, 0x00, 0x00, 0x00};

    CFTW0_DATA_START[3] = (uint8_t)Start_Freq_word;
    CFTW0_DATA_START[2] = (uint8_t)(Start_Freq_word >> 8);
    CFTW0_DATA_START[1] = (uint8_t)(Start_Freq_word >> 16);
    CFTW0_DATA_START[0] = (uint8_t)(Start_Freq_word >> 24);
    WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA_START, 0);

    CFTW0_DATA_STOP[3] = (uint8_t)Stop_Freq_Word;
    CFTW0_DATA_STOP[2] = (uint8_t)(Stop_Freq_Word >> 8);
    CFTW0_DATA_STOP[1] = (uint8_t)(Stop_Freq_Word >> 16);
    CFTW0_DATA_STOP[0] = (uint8_t)(Stop_Freq_Word >> 24);
    WriteData_AD9959(CW1, 4, CFTW0_DATA_STOP, 0);

    RDW_DATA[3] = (uint8_t)Step_Word;
    RDW_DATA[2] = (uint8_t)(Step_Word >> 8);
    RDW_DATA[1] = (uint8_t)(Step_Word >> 16);
    RDW_DATA[0] = (uint8_t)(Step_Word >> 24);
    WriteData_AD9959(RDW_ADD, 4, RDW_DATA, 0);

    uint8_t FDW_DATA[4] = {0x00, 0x00, 0x00, 0x00};
    FDW_DATA[3] = (uint8_t)Step_Word;
    FDW_DATA[2] = (uint8_t)(Step_Word >> 8);
    FDW_DATA[1] = (uint8_t)(Step_Word >> 16);
    FDW_DATA[0] = (uint8_t)(Step_Word >> 24);
    WriteData_AD9959(FDW_ADD, 4, FDW_DATA, 0);

    uint8_t LSRR_DATA[2] = {0x00, 0x00};
    LSRR_DATA[1] = (uint8_t)Time_word;
    LSRR_DATA[0] = (uint8_t)Time_word;
    WriteData_AD9959(LSRR_ADD, 2, LSRR_DATA, 1); // UPDATE

    switch (Channel)
    {
    case 0:
        WRT(PS0, 1);
    case 1:
        WRT(PS1, 1);
    case 2:
        WRT(PS2, 1);
    case 3:
        WRT(PS3, 1);
    }
}

uint32_t Get_Frequency(void)
{
    uint8_t data[4] = {0};
    ReadData_AD9959(CFTW0_ADD, 4, data);
    int Freq = (data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3]) / 8.589934592;
    return (Freq + 1) / 20;
}

uint8_t Get_Amplitude(void)
{
    uint8_t data[3] = {0};
    ReadData_AD9959(ACR_ADD, 3, data);
    return data[2];
}

void ReadData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData)
{
    uint8_t ControlValue = 0;
    uint8_t ValueToRead = 0;
    uint8_t RegisterIndex = 0;
    uint8_t i = 0;
    ControlValue = RegisterAddress | 0x80;
    // Write address
    WRT(CS, 0);
    WRT(SCLK, 0);
    delay_9959(0x20);
    for (i = 0; i < 8; i++)
    {
        WRT(SCLK, 0);
        if (0x80 == (ControlValue & 0x80))
            WRT(SDIO0, 1);
        else
            WRT(SDIO0, 0);
        WRT(SCLK, 1);
        ControlValue <<= 1;
        delay_9959(2);
    }

    WRT(SCLK, 0);

    // �л�������ģʽ
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = AD9959_SD0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AD9959_SD0_GPIO_Port, &GPIO_InitStruct);
    delay_9959(0x20);
    // Read data
    // ÿ���ֽڵ����ݶ��ǴӸ�λ��ʼ����
    // ��һ��������ֽ������λ���ֽ�
    for (RegisterIndex = 0; RegisterIndex < NumberofRegisters; RegisterIndex++)
    {
        ValueToRead = 0;
        for (i = 0; i < 8; i++)
        {
            ValueToRead <<= 1;
            WRT(SCLK, 1);
            if (GET(SDIO0) == 1)
                ValueToRead |= 0x01;
            else
                ValueToRead &= 0xFE;
            WRT(SCLK, 0);
        }
        WRT(SCLK, 0);
        RegisterData[RegisterIndex] = ValueToRead;
    }
    delay_9959(0x20);
    WRT(CS, 1);
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(AD9959_SD0_GPIO_Port, &GPIO_InitStruct);
}

//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__AD9959_ENABLE__














