#include "ctrl.h"

int main()
{
	//*********************�����Դ�Ӳ����Ϣ******************************************//
	//��ȡ���̾��
	HANDLE hDisk = GetDiskHandle(0);
	//��ȡ���̷�������
	int iPartitions = GetPartitionNumber(hDisk);
	printf("����0��Ч�ķ�����Ϊ%d��\n", iPartitions);
	//��������б�������ڴ�ռ�
	Partition_Info* PartitionList = (Partition_Info*)malloc(sizeof(Partition_Info)*iPartitions);
	//��ȡ�����б�
	GetPartitions(hDisk, PartitionList);
	//��ʾ�����б�
	ShowPartitions(PartitionList, iPartitions);

	//*********************�ƶ�Ӳ����Ϣ******************************************//
	//��ȡ���̾��
	HANDLE hDisk2 = GetDiskHandle(1);
	//��ȡ���̷�������
	int iPartitions2 = GetPartitionNumber(hDisk2);
	printf("\n����1��Ч�ķ�����Ϊ%d��\n", iPartitions2);
	//��������б�������ڴ�ռ�
	Partition_Info* PartitionList2 = (Partition_Info*)malloc(sizeof(Partition_Info)*iPartitions2);
	//��ȡ�����б�
	GetPartitions(hDisk2, PartitionList2);
	//��ʾ�����б�
	ShowPartitions(PartitionList2, iPartitions2);
	//�ͷŴ����б�
	free(PartitionList);
	free(PartitionList2);
	getchar();
	return 0;
}