#include <stdint.h>
typedef unsigned __int64 uint48_t;

//**********************GPT��ؽṹ��******************************//
// LBA1��GPTͷ�ṹ��
typedef struct
{
	uint8_t EFIPARTSign[8];			   //�̶��ַ�����EFI PART��
	uint8_t GPTVersion[4];			   //GPT�汾��
	uint8_t PrimaryGPTHeaderLength[4]; //��GPTͷ��С
	uint8_t PrimaryGPTHeaderCRC32[4];  //��GPTͷ��CRC32У������
	uint8_t Reserve1[4];			   //����
	uint8_t PrimaryGPTHeaderStart[8];  //��GPTͷ��ʼ������
	uint8_t PrimaryGPTHeaderBackup[8]; //��GPTͷ����λ��������
	uint8_t GPTPartitionStart[8];	   //GPT������ʼ������
	uint8_t GPTPartitionEnd[8];		   //GPT��������������
	uint8_t DiskGUID[16];			   //���̵�GUID
	uint8_t PartitionStart[8];		   //��������ʼ������
	uint8_t PartitionTables[4];		   //����������Ŀ
	uint8_t BytesPerPartitionTable[4]; //ÿ��������ռ���ֽ���
	uint8_t PartitionTableCRC32[4];	   //������CRCУ������
	uint8_t Reserve2[420];			   //����
}PGPTH_Byte;

typedef struct
{
	uint32_t Part1;					 //GUID��1����
	uint16_t Part2;					 //GUID��2����
	uint16_t Part3;					 //GUID��3����
	uint16_t Part4;					 //GUID��4����
	uint48_t Part5;					 //GUID��5����
}GUID_Info;

// LBA1(GPTͷ�ṹ��)��С��ģʽת��Ϊ���ģʽ֮��Ľṹ�壩
typedef struct
{
	uint8_t   GPTVersion[4];          //GPT�汾��
	uint32_t  PrimaryGPTHeaderLength; //��GPTͷ��С
	uint32_t  PrimaryGPTHeaderCRC32;  //��GPTͷ��CRC32У������
	uint64_t  PrimaryGPTHeaderStart;  //��GPTͷ��ʼ������
	uint64_t  PrimaryGPTHeaderBackup; //��GPTͷ����λ��������
	uint64_t  GPTPartitionStart;      //GPT������ʼ������
	uint64_t  GPTPartitionEnd;        //GPT��������������
	GUID_Info DiskGUID;				  //���̵�GUID
	uint64_t  PartitionStart;         //��������ʼ������
	uint32_t  PartitionTables;        //����������Ŀ
	uint32_t  BytesPerPartitionTable; //ÿ��������ռ���ֽ���
	uint32_t  PartitionTableCRC32;    //������CRCУ������
}PGPTH_Info;
//********************************************************************//

//**********************GPTEntry��ؽṹ��******************************//
typedef struct
{
	uint8_t TypeGUID[16];           //��GUID��ʾ�ķ�������
	uint8_t UniqueGUID[16];			//��GUID��ʾ�ķ���Ψһ��ʾ��
	uint8_t SectorStart[8];			//�÷�������ʼ��������LBAֵ��ʾ
	uint8_t SectorEnd[8];			//�÷����Ľ�������(����)����LBAֵ��ʾ��ͨ��������
	uint8_t PartitionAttrib[8];		//�÷��������Ա�־
	WCHAR   PartitionName[36];		//UTF-16LE���������ɶ��ķ������ƣ����32���ַ���
}GPTEntry_Byte;

typedef struct
{
	GUID_Info TypeGUID;				//��GUID��ʾ�ķ�������
	GUID_Info UniqueGUID;			//��GUID��ʾ�ķ���Ψһ��ʾ��
	uint64_t  SectorStart;			//�÷�������ʼ��������LBAֵ��ʾ
	uint64_t  SectorEnd;			//�÷����Ľ�������(����)����LBAֵ��ʾ��ͨ��������
	uint64_t  PartitionAttrib;		//�÷��������Ա�־
	WCHAR     PartitionName[36];	//UTF-16LE���������ɶ��ķ������ƣ����32���ַ���
}GPTEntry_Info;
//********************************************************************//

//**********************Partition��ؽṹ��***************************//
// ���̷������ͽṹ��
typedef enum
{
	fsptNullType = 0x00,
	fsptFAT32 = 0x01,
	fsptXENIX__root = 0X02,
	fsptXENIX_usr = 0X03,
	fsptFAT16_32M = 0X04,
	fsptExtended = 0X05,
	fsptFAT16 = 0X06,
	fsptHPFS_NTFS = 0X07,
	fsptAIX = 0X08,
	fsptAIX_bootable = 0X09,
	fsptOS_2_Boot_Manage = 0X0A,
	fsptWin95_FAT32 = 0X0B,
	fsptWin95_Fat32 = 0X0C,
	fsptWin95_FAT16 = 0X0E,
	fsptWin95_Extended_8GB = 0X0F,
	fsptOPUS = 0X10,
	fsptHidden_FAT12 = 0X11,
	fsptCompaq_diagnost = 0X12,
	fsptHidden_FAT16 = 0X16,
	fsptHidden_FAT16_32GB = 0X14,
	fsptHidden_HPFS_NTFS = 0X17,
	fsptAST_Windows_swap = 0X18,
	fsptHidden_FAT32 = 0X1B,
	fsptHidden_FAT32_partition = 0X1C,
	fsptHidden_LBA_VFAT_partition = 0X1E,
	fsptNEC_DOS = 0X24,
	fsptPartition_Magic = 0X3C,
	fsptVenix_80286 = 0X40,
	fsptPPC_PreP_Boot = 0X41,
	fsptSFS = 0X42,
	fsptQNX4_x = 0X4D,
	fsptQNX4_x_2nd_part = 0X4E,
	fsptQNX4_x_3rd_part = 0X4F,
	fsptOntrack_DM = 0X50,
	fsptOntrack_DM6_Aux = 0X51,
	fsptCP_M = 0X52,
	fsptOnTrack_DM6_AUX = 0X53,
	fsptOnTrack_DM6 = 0X54,
	fsptEZ_Drive = 0X55,
	fsptGolden_Bow = 0X56,
	fsptPriam_Edisk = 0X5C,
	fsptSpeed_Stor = 0X61,
	fsptGNU_HURD_or_Sys = 0X63,
	fsptNovell_Netware = 0X64,
	fsptNovell_NetWare = 0X65,
	fsptDisk_Secure_Mult = 0X70,
	fsptPC_IX = 0X75,
	fsptOld_Minix = 0X80,
	fsptMinix_Old_Linux = 0X81,
	fsptLinux_swap = 0X82,
	fsptLinux = 0X83,
	fsptOS_2_hidden_C = 0X84,
	fsptLinux_extended = 0X85,
	fsptNTFS_volume_set = 0X86,
	fsptNTFS_Volume_Set = 0X87,
	fsptAmoeba = 0X93,
	fsptAmoeba_BBT = 0X94,
	fsptIBM_Thinkpad_hidden = 0XA0,
	fsptBSD_386 = 0XA5,
	fsptOpen_BSD = 0XA6,
	fsptNextSTEP = 0XA7,
	fsptBSDI_fs = 0XB7,
	fsptBSDI_swap = 0XB8,
	fsptSolaris_boot_partition = 0XBE,
	fsptDRDOS_NovellDOS_secured_Partition = 0XC0,
	fsptDRDOS_sec = 0XC1,
	fsptDRDOS_Sec = 0XC4,
	fsptDRDOS_SEC = 0XC6,
	fsptSyrinx = 0XC7,
	fsptCP_M_CTOS = 0XDB,
	fsptDOS_access = 0XE1,
	fsptDOS_R_O = 0XE3,
	fsptSpeedStor = 0XE4,
	fsptBeOS_fs = 0XEB,
	fsptGPT = 0XEE,
	fsptSpeedstor = 0XF1,
	fsptDOS3_3_secondary_partition = 0XF2,
	fsptSpeed_stor = 0XF4,
	fsptLAN_step = 0XFE,
	fsptBBT = 0XFF
}Partition_Type;

// ���̷�����Ϣ�ṹ��
typedef struct
{
	Partition_Type Type;        //��������
	uint64_t       SectorStart; //��ʼ����
	uint64_t       SectorEnd;   //��ֹ����
}Partition_Info;
//********************************************************************//

//**********************DPT��ؽṹ��********************************//
typedef struct
{
	uint8_t ActivePartition;    //�����
	uint8_t StartInfo[3];       //����������ʼ��ͷ�š������š������
	uint8_t PartitionType;      //��������
	uint8_t EndInfo[3];         //�������Ľ�����ͷ�š������š������
	uint8_t UsedSector[4];      //������ǰ��ʹ�õ�������
	uint8_t TotalSector[4];     //����������������
}DPT_Type;

typedef struct
{
	uint8_t  BootCode[446];     //ǰ��446�ֽ�
	DPT_Type DPT_Table[4];
	uint8_t  EndSign[2];        //�������
}DPT_Byte;

// ������DPT�ṹ��
typedef struct
{
	bool           ActivePartition;      //�����
	uint8_t        StartMagneticHead;    //��ʼ��ͷ��
	uint8_t        StartSector;          //��ʼ������
	uint16_t       StartCylinder;        //��ʼ�����
	Partition_Type PartitionType;        //��������
	uint8_t        EndMagneticHead;      //������ͷ��
	uint8_t        EndSector;            //����������
	uint16_t       EndCylinder;          //���������
	uint32_t       UsedSector;           //������ǰ��ʹ�õ�������
	uint32_t       TotalSector;          //����������������
	uint32_t       MBROffset;            //��������MBRƫ����
}DPT_Info;
//********************************************************************//

static const GUID_Info GUID_ptUnuse = { 0x00000000, 0x0000, 0x0000, 0x0000, 0x000000000000 }; //δʹ��