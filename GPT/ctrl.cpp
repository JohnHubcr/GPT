#include"ctrl.h"

// ��������ֻ��һ������������, ��0x1C2����¼��ϵͳ��Ϣ�����0xEE��Ӳ�̲���GPT�������������MBR����
bool PartitionTableDetermination(unsigned char* SectorData)

{
	if (SectorData[0x1C2] == 0xEE)

		return PDT_GPT;
	else
		return PDT_MBR;
}

uint16_t ArrayToU16LittleEnd(uint8_t *str)
{
	return (str[0] + (str[1] << 8));
}

uint16_t ArrayToU16BigEnd(uint8_t *str)
{
	return (str[1] + (str[0] << 8));
}

uint32_t ArrayToU32LittleEnd(uint8_t *str)
{
	return (str[0] + (str[1] << 8) + (str[2] << 16) + (str[3] << 24));
}

uint32_t ArrayToU32BigEnd(uint8_t *str)
{
	return (str[3] + (str[2] << 8) + (str[1] << 16) + (str[0] << 24));
}

uint64_t ArrayToU48LittleEnd(uint8_t *str)
{
	return  ((uint64_t)str[0] +
		((uint64_t)str[1] << 8) +
		((uint64_t)str[2] << 16) +
		((uint64_t)str[3] << 24) +
		((uint64_t)str[4] << 32) +
		((uint64_t)str[5] << 40));
}

uint64_t ArrayToU48BigEnd(uint8_t *str)
{
	return  ((uint64_t)str[5] +
		((uint64_t)str[4] << 8) +
		((uint64_t)str[3] << 16) +
		((uint64_t)str[2] << 24) +
		((uint64_t)str[1] << 32) +
		((uint64_t)str[0] << 40));
}

uint64_t ArrayToU64LittleEnd(uint8_t *str)
{
	return  ((uint64_t)str[0] +
		((uint64_t)str[1] << 8) +
		((uint64_t)str[2] << 16) +
		((uint64_t)str[3] << 24) +
		((uint64_t)str[4] << 32) +
		((uint64_t)str[5] << 40) +
		((uint64_t)str[6] << 48) +
		((uint64_t)str[7] << 56));
}

uint64_t ArrayToU64BigEnd(uint8_t *str)
{
	return  ((uint64_t)str[7] +
		((uint64_t)str[6] << 8) +
		((uint64_t)str[5] << 16) +
		((uint64_t)str[4] << 24) +
		((uint64_t)str[3] << 32) +
		((uint64_t)str[2] << 40) +
		((uint64_t)str[1] << 48) +
		((uint64_t)str[0] << 56));
}

// ͨ���������̵ı�ţ���ȡ�����̵ľ��
HANDLE GetDiskHandle(int iDiskNo)
{
	char szDriverBuffer[128];
	memset(szDriverBuffer, 0, 128);
	//��ʽ���豸�ļ�����
	sprintf(szDriverBuffer, "\\\\.\\PhysicalDrive%d", iDiskNo);
	HANDLE m_hDevice = NULL;
	//CreateFile��ȡ���豸���
	m_hDevice = CreateFileA(
		szDriverBuffer,// �豸����,����ָ��һ��Ӳ�̣����Ӳ�̵��Լ��޸ľͺ���
		GENERIC_READ, // ָ�������ʷ�ʽ
		FILE_SHARE_READ | FILE_SHARE_WRITE, // ����ģʽΪ��|д��0��ʾ���ܹ���
		NULL, // NULL��ʾ�þ�����ܱ��ӳ���̳�
		OPEN_EXISTING, // ���Ѿ����ڵ��ļ����ļ���������������ʧ��
		NULL, // ָ���ļ�����
		NULL);
	if (m_hDevice == INVALID_HANDLE_VALUE)
	{
		m_hDevice = NULL;
		//��Ч
		return INVALID_HANDLE_VALUE;
	}
	//�豸���
	return m_hDevice;
}

// ��ȡ��������
BOOL ReadSectorData(HANDLE& hDevice, uint64_t redpos, uint8_t* lpOutBuffer, uint32_t ReadLen)
{
	memset(lpOutBuffer, 0, ReadLen);
	LARGE_INTEGER li;
	li.QuadPart = redpos * 512;//����������ֽڵ�ַ��ͨ�����ö�ȡ�ĵ�ַ�ͳ��Ƚ���read
	SetFilePointer(hDevice, li.LowPart, &li.HighPart, FILE_BEGIN);
	DWORD DCount = 0; //����
	BOOL bResult = ReadFile(hDevice, lpOutBuffer, ReadLen, &DCount, NULL);
	return (BOOL)bResult;
}

// ���������ֽ����е�DPT_Byteת��Ϊ4��ϡ��ṹ��DPT_Info�Ա��ڳ����ȡ��Ϣ
void GetDPTInfo(DPT_Byte* src, DPT_Info* dest)
{
	for (int i = 0; i < 4; i++)
	{
		dest[i].ActivePartition = src->DPT_Table[i].ActivePartition == 0x80 ? true : false;//0x80�ǻ������0x00�Ƿǻ����
		dest[i].StartMagneticHead = src->DPT_Table[i].StartInfo[0];
		dest[i].StartSector = src->DPT_Table[i].StartInfo[1] & 0x3F;
		dest[i].StartCylinder = src->DPT_Table[i].StartInfo[2] + ((src->DPT_Table[i].StartInfo[1] & 0xC0) << 2);
		dest[i].PartitionType = (Partition_Type)src->DPT_Table[i].PartitionType;
		dest[i].EndMagneticHead = src->DPT_Table[i].EndInfo[0];
		dest[i].EndSector = src->DPT_Table[i].EndInfo[1] & 0x3F;
		dest[i].EndCylinder = src->DPT_Table[i].EndInfo[2] + ((src->DPT_Table[i].EndInfo[1] & 0xC0) << 2);
		dest[i].UsedSector = ArrayToU32LittleEnd(src->DPT_Table[i].UsedSector);
		dest[i].TotalSector = ArrayToU32LittleEnd(src->DPT_Table[i].TotalSector);

		dest[i].MBROffset = dest[i].UsedSector;
	}
}

// ��ȡָ�����̵Ĵ��̷�����DPT
void GetDPT(HANDLE hDisk, DPT_Info* DPT)
{
	DPT_Byte DPTbuffer;//����DPT������Ϊbuffer��ע��MCU��ջ�ߴ�Ҫ����512�ֽ�
	ReadSectorData(hDisk, 0, (uint8_t*)(void*)&DPTbuffer, sizeof(DPT_Byte));//����DPT
	GetDPTInfo(&DPTbuffer, DPT);
}

// ����DPT�ķ��������жϴ��̷�������GPT��MBR
int DPTDetermination(DPT_Info* DPT)
{
	switch (DPT->PartitionType)
	{
	case fsptNullType:
		return DPT_NULL;
	case fsptGPT:
		return DPT_GPT;
	case fsptExtended:
	case fsptWin95_Extended_8GB:
		return DPT_EBR;
	default:
		return DPT_MBR;
	}
}

// ��ȡһ��MBR��ʽ�Ĵ�������Ч�־�������ʱ����EBR��Ҳ����˵���4��MBR
int GetPartitionNumberOfMBR(HANDLE hDisk, DPT_Info* DPT)
{
	DPT_Byte DPTsector;
	DPT_Info DPTinfo[4];
	uint64_t sectorsoffset = 0;
	ReadSectorData(hDisk, 0, (uint8_t*)(void*)&DPTsector, 512);//����DPT
	GetDPTInfo(&DPTsector, DPTinfo);//С��ģʽת����������Ϊ������Ϣ
	int ValidPartitions = 0;
	for (int i = 0; i < 4; i++)
	{
		if (DPTDetermination(&(DPTinfo[i])) == DPT_NULL)
			break;
		else
			ValidPartitions++;
	}
	return ValidPartitions;
}

static inline GUID_Info ArrayToGUID(uint8_t* str)
{
	GUID_Info guid;
	guid.Part1 = ArrayToU32LittleEnd(&(str[0]));
	guid.Part2 = ArrayToU16LittleEnd(&(str[4]));
	guid.Part3 = ArrayToU16LittleEnd(&(str[6]));
	guid.Part4 = ArrayToU16BigEnd(&(str[8]));
	guid.Part5 = ArrayToU48BigEnd(&(str[10]));
	return guid;
}

// ���������ֽ����е�PGPTH_Byteת��Ϊϡ��ṹ��PGPTH_Info�Ա��ڳ����ȡ��Ϣ
void GetPGPTHInfo(PGPTH_Byte* src, PGPTH_Info* dest)
{
	memcpy(dest->GPTVersion, src->GPTVersion, 4);
	dest->PrimaryGPTHeaderLength = ArrayToU32LittleEnd(src->PrimaryGPTHeaderLength);
	dest->PrimaryGPTHeaderCRC32 = ArrayToU32LittleEnd(src->PrimaryGPTHeaderCRC32);
	dest->PrimaryGPTHeaderStart = ArrayToU64LittleEnd(src->PrimaryGPTHeaderStart);
	dest->PrimaryGPTHeaderBackup = ArrayToU64LittleEnd(src->PrimaryGPTHeaderBackup);
	dest->GPTPartitionStart = ArrayToU64LittleEnd(src->GPTPartitionStart);
	dest->GPTPartitionEnd = ArrayToU64LittleEnd(src->GPTPartitionEnd);
	dest->DiskGUID = ArrayToGUID(src->DiskGUID);
	dest->PartitionStart = ArrayToU64LittleEnd(src->PartitionStart);
	dest->PartitionTables = ArrayToU32LittleEnd(src->PartitionTables);
	dest->BytesPerPartitionTable = ArrayToU32LittleEnd(src->BytesPerPartitionTable);
	dest->PartitionTableCRC32 = ArrayToU32LittleEnd(src->PartitionTableCRC32);
}

// ��ȡָ�����̵���GPTͷ(Primary GPT Header,�����м�ΪPGPTH)
void GetPGPTH(HANDLE hDisk, PGPTH_Info* PGPTH)

{
	PGPTH_Byte PGPTHbuffer;
	ReadSectorData(hDisk, 1, (uint8_t*)(void*)&PGPTHbuffer, sizeof(PGPTH_Byte));
	GetPGPTHInfo(&PGPTHbuffer, PGPTH);
}

// ��ȡ�������ݣ������Դ���������ȡ
BOOL ReadDiskData(HANDLE& hDevice, uint64_t redpos, uint8_t* lpOutBuffer, uint32_t ReadLen)
{
	memset(lpOutBuffer, 0, ReadLen);
	uint8_t buffer[SECTOR_SIZE];
	BOOL bReturn = ReadSectorData(hDevice, redpos / SECTOR_SIZE, buffer, SECTOR_SIZE);
	for (int i = 0; i < ReadLen; i++)
		lpOutBuffer[i] = buffer[i + (redpos % SECTOR_SIZE)];
	return bReturn;
}

// ���������ֽ����е�GPTEntry_Byteת��Ϊϡ��ṹ��GPTEntry_Info�Ա��ڳ����ȡ��Ϣ
void GetGPTEInfo(GPTEntry_Byte* src, GPTEntry_Info* dest)
{
	for (int i = 0; i < 1; i++)
	{
		dest[i].TypeGUID = ArrayToGUID(src[i].TypeGUID);
		dest[i].UniqueGUID = ArrayToGUID(src[i].UniqueGUID);
		dest[i].SectorStart = ArrayToU64LittleEnd(src[i].SectorStart);
		dest[i].SectorEnd = ArrayToU64LittleEnd(src[i].SectorEnd);
		dest[i].PartitionAttrib = ArrayToU64BigEnd(src[i].PartitionAttrib);
		memcpy(dest[i].PartitionName, src[i].PartitionName, 72);
	}
}

// �ж�����GUID_Info�Ƿ����
bool GUIDcmp(GUID_Info* guid1, GUID_Info* guid2)
{
	if (guid1->Part1 != guid2->Part1)
		return false;
	if (guid1->Part2 != guid2->Part2)
		return false;
	if (guid1->Part3 != guid2->Part3)
		return false;
	if (guid1->Part4 != guid2->Part4)
		return false;
	if (guid1->Part5 != guid2->Part5)
		return false;
	return true;
}

// ��ȡһ��GPT��ʽ�Ĵ�������Ч�־���
int GetPartitionNumberOfGPT(HANDLE hDisk, DPT_Info* DPT)
{
	if (DPTDetermination(DPT) == DPT_MBR)
		return -1;
	GPTEntry_Byte GPTEbuffer;
	GPTEntry_Info GPTEinfo;
	PGPTH_Info PGPTH;
	GetPGPTH(hDisk, &PGPTH);
	int ValidPartitions = 0;
	for (int i = 0; i < PGPTH.PartitionTables; i++)
	{
		ReadDiskData(
			hDisk, //��ȡ���̾��
			SECTOR_SIZE * PGPTH.PartitionStart + i * PGPTH.BytesPerPartitionTable, //�����ȡ��GPTEλ��
			(uint8_t*)(void*)&GPTEbuffer, //��������ַ
			sizeof(GPTEntry_Byte)); //�ֽ���
		GetGPTEInfo(&GPTEbuffer, &GPTEinfo);
		if (GUIDcmp(&(GPTEinfo.TypeGUID), (GUID_Info*)&GUID_ptUnuse))
			break;
		else
			ValidPartitions++;
	}
	return ValidPartitions;
}

// ��ȡ���̷��������� ����EBR
int GetPartitionNumber(HANDLE hDisk)
{
	DPT_Info DPT[4];
	GetDPT(hDisk, DPT);
	if (DPTDetermination(DPT) == DPT_MBR)
		return GetPartitionNumberOfMBR(hDisk, DPT);
	else if (DPTDetermination(DPT) == DPT_GPT)
		return GetPartitionNumberOfGPT(hDisk, DPT);
	else
		return -1;
}

// ��ȡָ�����̵�ָ��LBA��Ϣ
void GetGPTE(HANDLE hDisk, PGPTH_Info* PGPTH, GPTEntry_Info* GPTE, uint8_t GPTESerial)
{
	GPTEntry_Byte GPTEbuffer;
	ReadDiskData(hDisk, PGPTH->PartitionStart * SECTOR_SIZE + PGPTH->BytesPerPartitionTable * GPTESerial, (uint8_t*)(void*)&GPTEbuffer, PGPTH->BytesPerPartitionTable);
	GetGPTEInfo(&GPTEbuffer, GPTE);
}

// ��ȡ�־���Ϣ����ʱ����EBR
int GetPartitions(HANDLE hDisk, Partition_Info* Partitions)
{
	DPT_Info DPT[4];
	GetDPT(hDisk, DPT);
	int iPartitions = GetPartitionNumber(hDisk);
	if (DPTDetermination(&(DPT[0])) == DPT_GPT)
	{
		PGPTH_Info PGPTH;
		GetPGPTH(hDisk, &PGPTH);
		GPTEntry_Info GPTE;
		for (int i = 0; i < iPartitions; i++)
		{
			GetGPTE(hDisk, &PGPTH, &GPTE, i);
			Partitions[i].Type = (Partition_Type)0;
			Partitions[i].SectorStart = GPTE.SectorStart;
			Partitions[i].SectorEnd = GPTE.SectorEnd;
		}
	}
	else if (DPTDetermination(&(DPT[0])) == DPT_MBR)
	{
		for (int i = 0; i < iPartitions; i++)
		{
			Partitions[i].Type = (Partition_Type)0;
			Partitions[i].SectorStart = DPT[i].UsedSector;
			Partitions[i].SectorEnd = DPT[i].UsedSector + DPT[i].TotalSector;
		}
	}

	return iPartitions;
}

// ��ʾ�����б�
void ShowPartitions(Partition_Info* Partitions, int len)
{
	printf("----------------------------------------------------------------------------\n");
	printf("����    ����      ��������      ��ʼ����      ��ֹ����              ������С\n");
	printf("----------------------------------------------------------------------------\n");
	for (int i = 0; i < len; i++)
	{
		printf("%3d", i);
		printf("%8d", Partitions[i].Type);
		printf("%15llu", Partitions[i].SectorEnd - Partitions[i].SectorStart);
		printf("%14llu", Partitions[i].SectorStart);
		printf("%14llu", Partitions[i].SectorEnd);
		uint64_t PartitionSize = (Partitions[i].SectorEnd - Partitions[i].SectorStart) * 512;
		printf("%16llu Bytes\n", PartitionSize);
	}
}