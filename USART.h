#include "main.h"
bool openPort(char *portname);
bool SetUsart(int Bitrate);
bool SetupTimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier, DWORD
	ReadTotalConstant, DWORD WriteTotalMultiplier, DWORD WriteTotalConstant);
bool USART_send(char* WriteBuffer, DWORD dwSend, HANDLE Comm);
void Usart_Arm_Control(char* port_name, int bitrate, int num_of_chess, Point src_position[], Point dst_position[]);