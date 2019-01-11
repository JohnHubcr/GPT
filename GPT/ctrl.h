#include <windows.h>
#include <stdio.h>
#include "data.h"

#define PDT_GPT 0
#define PDT_MBR 0

#define DPT_NULL 0
#define DPT_GPT  1
#define DPT_MBR  2
#define DPT_EBR  3

#define SECTOR_SIZE 512

// ��������ֻ��һ������������, ��0x1C2����¼��ϵͳ��Ϣ�����0xEE��Ӳ�̲���GPT�������������MBR����
bool PartitionTableDetermination(unsigned char* SectorData);

uint16_t ArrayToU16LittleEnd(uint8_t *str);
uint16_t ArrayToU16BigEnd(uint8_t *str);
uint32_t ArrayToU32LittleEnd(uint8_t *str);
uint64_t ArrayToU48LittleEnd(uint8_t *str);
uint64_t ArrayToU48BigEnd(uint8_t *str);
uint64_t ArrayToU64LittleEnd(uint8_t *str);
uint64_t ArrayToU64BigEnd(uint8_t *str);

// ͨ���������̵ı�ţ���ȡ�����̵ľ��
HANDLE GetDiskHandle(int iDiskNo);

// ��ȡ��������
BOOL ReadSectorData(HANDLE& hDevice, uint64_t redpos, uint8_t* lpOutBuffer, uint32_t ReadLen);

// ���������ֽ����е�DPT_Byteת��Ϊ4��ϡ��ṹ��DPT_Info�Ա��ڳ����ȡ��Ϣ
void GetDPTInfo(DPT_Byte* src, DPT_Info* dest);

// ��ȡָ�����̵Ĵ��̷�����DPT
void GetDPT(HANDLE hDisk, DPT_Info* DPT);

// ����DPT�ķ��������жϴ��̷�������GPT��MBR
int DPTDetermination(DPT_Info* DPT);

// ��ȡһ��MBR��ʽ�Ĵ�������Ч�־�������ʱ����EBR��Ҳ����˵���4��MBR
int GetPartitionNumberOfMBR(HANDLE hDisk, DPT_Info* DPT);

// ���������ֽ����е�PGPTH_Byteת��Ϊϡ��ṹ��PGPTH_Info�Ա��ڳ����ȡ��Ϣ
void GetPGPTHInfo(PGPTH_Byte* src, PGPTH_Info* dest);

static inline GUID_Info ArrayToGUID(uint8_t* str);

// ��ȡָ�����̵���GPTͷ(Primary GPT Header,�����м�ΪPGPTH)
void GetPGPTH(HANDLE hDisk, PGPTH_Info* PGPTH);

// ��ȡ�������ݣ������Դ���������ȡ
BOOL ReadDiskData(HANDLE& hDevice, uint64_t redpos, uint8_t* lpOutBuffer, uint32_t ReadLen);

// ��ȡһ��GPT��ʽ�Ĵ�������Ч�־���
int GetPartitionNumberOfGPT(HANDLE hDisk, DPT_Info* DPT);

// ��ȡ���̷��������� ����EBR
int GetPartitionNumber(HANDLE hDisk);

// ��ȡָ�����̵�ָ��LBA��Ϣ
void GetGPTE(HANDLE hDisk, PGPTH_Info* PGPTH, GPTEntry_Info* GPTE, uint8_t GPTESerial);

// ��ȡ�־���Ϣ����ʱ����EBR
int GetPartitions(HANDLE hDisk, Partition_Info* Partitions);

//��ʾ�����б�
void ShowPartitions(Partition_Info* Partitions, int len);