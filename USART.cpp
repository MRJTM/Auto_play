#include "USART.h"

HANDLE com;
OVERLAPPED OverLapped;

/*----打开串口设备----*/
bool openPort(char *portname)
{
	com = CreateFile(portname,			//串口号
		GENERIC_READ | GENERIC_WRITE,	//允许读写
		0,								//通讯设备以独占方式打开
		NULL,							//无安全属性
		OPEN_EXISTING,					//通讯设备已存在
		FILE_FLAG_OVERLAPPED,			//异步I/O
		NULL);							//通讯设备不能用模板打开
	if (com == INVALID_HANDLE_VALUE)
	{
		CloseHandle(com);
		printf("打开串口失败！\n");
		return false;
	}
	else
	{
		return true;
	}
}

/*----配置串口----*/
bool SetUsart(int Bitrate)
{
	SetupComm(com, 1024, 1024);//设置（读、写）缓冲区大小为1024
	DCB dcb;
	memset(&dcb, 0, sizeof(dcb));
	if (!GetCommState(com, &dcb))
	{
		return false;
	}
	else
	{
		// set DCB to configure the serial port
		//波特率为115200，无奇偶校验，8位数据位，1位停止位

		dcb.DCBlength = sizeof(dcb);
		// ---------- Serial Port Config -------
		dcb.BaudRate = Bitrate;					//波特率
		dcb.Parity = NOPARITY;					//无奇偶校验
		dcb.fParity = 0;						//关闭奇偶校验使能位	
		dcb.StopBits = ONESTOPBIT;				//1位停止位
		dcb.ByteSize = 8;						//8位数据位
		dcb.fOutxCtsFlow = 0;
		dcb.fOutxDsrFlow = 0;
		dcb.fDtrControl = DTR_CONTROL_DISABLE;
		dcb.fDsrSensitivity = 0;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
		dcb.fOutX = 0;
		dcb.fInX = 0;
		//---------- misc parameters -----
		dcb.fErrorChar = 0;
		dcb.fBinary = 1;
		dcb.fNull = 0;
		dcb.fAbortOnError = 0;
		dcb.wReserved = 0;
		dcb.XonLim = 2;
		dcb.XoffLim = 4;
		dcb.XonChar = 0x11;
		dcb.XoffChar = 0x13;
		dcb.EvtChar = 0;

		if (!SetCommState(com, &dcb))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

/*串口超市设置函数*/
bool SetupTimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier, DWORD
	ReadTotalConstant, DWORD WriteTotalMultiplier, DWORD WriteTotalConstant)
{
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = ReadInterval;                //读间隔时间超时 
	timeouts.ReadTotalTimeoutConstant = ReadTotalConstant;      //读时间常量
	timeouts.ReadTotalTimeoutMultiplier = ReadTotalMultiplier;  //读时间系数
	timeouts.WriteTotalTimeoutConstant = WriteTotalConstant;    //写时间常量    
	timeouts.WriteTotalTimeoutMultiplier = WriteTotalMultiplier;//写时间系数
	if (!SetCommTimeouts(com, &timeouts))
	{
		return false;
	}
	else
		return true;
}

/*----串口接收函数----*/

/*----串口发送函数----*/
bool USART_send(char* WriteBuffer, DWORD dwSend, HANDLE Comm)    //BYTE *szWriteBuffer
{
	bool bWrite = TRUE;
	bool bResult = TRUE;			//指示发送是否成功

	DWORD BytesSent = 0;
	HANDLE hWriteEvent = NULL;
	ResetEvent(hWriteEvent);

	//数据加载缓冲区进行发送
	if (bWrite)
	{
		OverLapped.Offset = 0;
		OverLapped.OffsetHigh = 0;
		bResult = WriteFile(com,    //通信设备句柄，CreateFile()返回值得到  
			WriteBuffer,             //指向写入数据缓冲区  
			dwSend,                  //设置要写的字节数  
			&BytesSent,              // 实际写入的字节数
			&OverLapped);            //指向异步I/O数据  
		if (!bResult)
		{
			DWORD dwError = GetLastError();
			switch (dwError)
			{
			case ERROR_IO_PENDING:
				BytesSent = 0;
				bWrite = FALSE;
				break;
			default:
				break;
			}
		}
	}
	if (!bWrite)
	{
		bWrite = TRUE;
		bResult = GetOverlappedResult(com,
			&OverLapped,
			&BytesSent,
			TRUE);
		if (!bResult)
		{
			std::cout << "GetOverlappedResults() in WriteFile()" << std::endl;
		}
	}
	if (BytesSent != dwSend)
	{
		std::cout << "WARNING: WriteFile() error.. Bytes Sent:" << BytesSent << "; Message Length: " << "5" << std::endl;
	}
	return TRUE;
}

void Usart_Arm_Control(char* port_name, int bitrate, int num_of_chess, Point src_position[], Point dst_position[])
{
	bool Openflag = false;
	bool flag_send = false;
	Openflag = openPort(port_name);
	if (Openflag)
	{
		printf("打开串口成功！\n");
		if (SetUsart(bitrate))//若配置串口成功就发送数据
		{
			printf("配置串口成功！\n");
			int src_x, src_y, dst_x, dst_y;

			for (int i = 0; i < num_of_chess; i++)
			{
				//设置要发送的内容
				src_x = src_position[i].x;
				src_y = src_position[i].y;
				dst_x = dst_position[i].x;
				dst_y = dst_position[i].y;
				char data[24] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'\n' };
				sprintf(data, "%s%s%d%s%d%s%d%s%d&", "a1", "x", src_x, "y", src_y, "X", dst_x, "Y", dst_y);
				
				//发送数据
				DWORD dwWrittenLen = 0;
				flag_send = USART_send(data, 24, com);

				//检测是否发送成功
				if (flag_send)
				{
					cout << "棋子" << i << "坐标发送成功" << endl;
				}
				else
				{
					cout << "棋子" << i << "坐标发送失败" << endl;
				}

				//每次操作延时5秒
				Sleep(5000);
			}
		}
		else
		{
			printf("配置串口失败！\n");
		}
	}
	else
	{
		printf("打开串口失败！\n");
	}
}