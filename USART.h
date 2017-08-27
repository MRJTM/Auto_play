#include "main.h"
bool openPort(char *portname);
bool SetUsart(int Bitrate);
bool SetupTimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier, DWORD
	ReadTotalConstant, DWORD WriteTotalMultiplier, DWORD WriteTotalConstant);
bool USART_send(char* WriteBuffer, DWORD dwSend, HANDLE Comm);
bool Usart_Arm_Control(Point &src_position, Point &dst_position);
int Open_Config_Usart(char* port_name, int bitrate);