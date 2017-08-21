#include "USART.h"

HANDLE com;
OVERLAPPED OverLapped;

/*----�򿪴����豸----*/
bool openPort(char *portname)
{
	com = CreateFile(portname,			//���ں�
		GENERIC_READ | GENERIC_WRITE,	//�����д
		0,								//ͨѶ�豸�Զ�ռ��ʽ��
		NULL,							//�ް�ȫ����
		OPEN_EXISTING,					//ͨѶ�豸�Ѵ���
		FILE_FLAG_OVERLAPPED,			//�첽I/O
		NULL);							//ͨѶ�豸������ģ���
	if (com == INVALID_HANDLE_VALUE)
	{
		CloseHandle(com);
		printf("�򿪴���ʧ�ܣ�\n");
		return false;
	}
	else
	{
		return true;
	}
}

/*----���ô���----*/
bool SetUsart(int Bitrate)
{
	SetupComm(com, 1024, 1024);//���ã�����д����������СΪ1024
	DCB dcb;
	memset(&dcb, 0, sizeof(dcb));
	if (!GetCommState(com, &dcb))
	{
		return false;
	}
	else
	{
		// set DCB to configure the serial port
		//������Ϊ115200������żУ�飬8λ����λ��1λֹͣλ

		dcb.DCBlength = sizeof(dcb);
		// ---------- Serial Port Config -------
		dcb.BaudRate = Bitrate;					//������
		dcb.Parity = NOPARITY;					//����żУ��
		dcb.fParity = 0;						//�ر���żУ��ʹ��λ	
		dcb.StopBits = ONESTOPBIT;				//1λֹͣλ
		dcb.ByteSize = 8;						//8λ����λ
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

/*���ڳ������ú���*/
bool SetupTimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier, DWORD
	ReadTotalConstant, DWORD WriteTotalMultiplier, DWORD WriteTotalConstant)
{
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = ReadInterval;                //�����ʱ�䳬ʱ 
	timeouts.ReadTotalTimeoutConstant = ReadTotalConstant;      //��ʱ�䳣��
	timeouts.ReadTotalTimeoutMultiplier = ReadTotalMultiplier;  //��ʱ��ϵ��
	timeouts.WriteTotalTimeoutConstant = WriteTotalConstant;    //дʱ�䳣��    
	timeouts.WriteTotalTimeoutMultiplier = WriteTotalMultiplier;//дʱ��ϵ��
	if (!SetCommTimeouts(com, &timeouts))
	{
		return false;
	}
	else
		return true;
}

/*----���ڽ��պ���----*/

/*----���ڷ��ͺ���----*/
bool USART_send(char* WriteBuffer, DWORD dwSend, HANDLE Comm)    //BYTE *szWriteBuffer
{
	bool bWrite = TRUE;
	bool bResult = TRUE;			//ָʾ�����Ƿ�ɹ�

	DWORD BytesSent = 0;
	HANDLE hWriteEvent = NULL;
	ResetEvent(hWriteEvent);

	//���ݼ��ػ��������з���
	if (bWrite)
	{
		OverLapped.Offset = 0;
		OverLapped.OffsetHigh = 0;
		bResult = WriteFile(com,    //ͨ���豸�����CreateFile()����ֵ�õ�  
			WriteBuffer,             //ָ��д�����ݻ�����  
			dwSend,                  //����Ҫд���ֽ���  
			&BytesSent,              // ʵ��д����ֽ���
			&OverLapped);            //ָ���첽I/O����  
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
		printf("�򿪴��ڳɹ���\n");
		if (SetUsart(bitrate))//�����ô��ڳɹ��ͷ�������
		{
			printf("���ô��ڳɹ���\n");
			int src_x, src_y, dst_x, dst_y;

			for (int i = 0; i < num_of_chess; i++)
			{
				//����Ҫ���͵�����
				src_x = src_position[i].x;
				src_y = src_position[i].y;
				dst_x = dst_position[i].x;
				dst_y = dst_position[i].y;
				char data[24] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'\n' };
				sprintf(data, "%s%s%d%s%d%s%d%s%d&", "a1", "x", src_x, "y", src_y, "X", dst_x, "Y", dst_y);
				
				//��������
				DWORD dwWrittenLen = 0;
				flag_send = USART_send(data, 24, com);

				//����Ƿ��ͳɹ�
				if (flag_send)
				{
					cout << "����" << i << "���귢�ͳɹ�" << endl;
				}
				else
				{
					cout << "����" << i << "���귢��ʧ��" << endl;
				}

				//ÿ�β�����ʱ5��
				Sleep(5000);
			}
		}
		else
		{
			printf("���ô���ʧ�ܣ�\n");
		}
	}
	else
	{
		printf("�򿪴���ʧ�ܣ�\n");
	}
}