#pragma once
#pragma warning(disable:4996)
#include <stdio.h>
#include "Struct.h"

#define MASTER_IND "master.ind"
#define MASTER_DATA "master.fl"
#define MASTER_GARBAGE "master_garbage.txt"
#define INDEXER_SIZE sizeof(struct Indexer)
#define MASTER_SIZE sizeof(struct Master)
#define SLAVE_DATA "slave.fl"
#define SLAVE_SIZE sizeof(struct Slave)


int insertMaster(struct Master rec)
{
	FILE* indexTable = fopen(MASTER_IND, "a+b");			
	FILE* database = fopen(MASTER_DATA, "a+b");				
	FILE* garbageZone = fopen(MASTER_GARBAGE, "rb");		
	struct Indexer indexer;
	
	long indexerSize = INDEXER_SIZE;

	fseek(indexTable, 0, SEEK_END);						

	if (ftell(indexTable))								
	{
		fseek(indexTable, -indexerSize, SEEK_END);		
		fread(&indexer, INDEXER_SIZE, 1, indexTable);	

		rec.id = indexer.id + 1;						
	}
	else                                              
	{
		rec.id = 1;								
	}

	rec.firstSlaveAddress = -1;
	rec.slavesCount = 0;

	fwrite(&rec, MASTER_SIZE, 1, database);				

	indexer.id = rec.id;									
	indexer.address = (rec.id - 1) * MASTER_SIZE;		
	indexer.exists = 1;										

	printf("Your master\'s id is %d\n", rec.id);

	fseek(indexTable, (rec.id - 1) * INDEXER_SIZE, SEEK_SET);
	fwrite(&indexer, INDEXER_SIZE, 1, indexTable);			
	fclose(indexTable);									
	fclose(database);
};

int getMaster(struct Master* master, int id)
{
	FILE* indexTable = fopen(MASTER_IND, "rb");				
	FILE* database = fopen(MASTER_DATA, "rb");				


	struct Indexer indexer;

	fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);	
	fread(&indexer, INDEXER_SIZE, 1, indexTable);			

	fseek(database, indexer.address, SEEK_SET);				
	fread(master, sizeof(struct Master), 1, database);		
	fclose(indexTable);										
	fclose(database);

	return 1;
}

int updateMaster(struct Master master)
{
	FILE* indexTable = fopen(MASTER_IND, "r+b");		
	FILE* database = fopen(MASTER_DATA, "r+b");				

	struct Indexer indexer;
	int id = master.id;


	fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);	
	fread(&indexer, INDEXER_SIZE, 1, indexTable);			

	fseek(database, indexer.address, SEEK_SET);				
	fwrite(&master, MASTER_SIZE, 1, database);				
	fclose(indexTable);										
	fclose(database);

	return 1;
}




int deleteMaster(int id)
{
	FILE* indexTable = fopen(MASTER_IND, "r+b");			


	struct Master master;
	getMaster(&master, id);

	struct Indexer indexer;

	fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);	
	fread(&indexer, INDEXER_SIZE, 1, indexTable);		

	indexer.exists = 0;										

	fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);

	fwrite(&indexer, INDEXER_SIZE, 1, indexTable);			
	fclose(indexTable);																		


	if (master.slavesCount)								
	{
		FILE* slavesDb = fopen(SLAVE_DATA, "r+b");
		struct Slave slave;

		fseek(slavesDb, master.firstSlaveAddress, SEEK_SET);

		for (int i = 0; i < master.slavesCount; i++)
		{
			fread(&slave, SLAVE_SIZE, 1, slavesDb);
			fclose(slavesDb);
			deleteSlave(master, slave, slave.trainId);

			slavesDb = fopen(SLAVE_DATA, "r+b");
			fseek(slavesDb, slave.nextAddress, SEEK_SET);
		}

		fclose(slavesDb);
	}
	return 1;
}

void info()
{
	FILE* indexTable = fopen("master.ind", "rb");

	if (indexTable == NULL)
	{
		printf("Error: database files are not created yet\n");
		return;
	}

	int masterCount = 0;
	int slaveCount = 0;

	fseek(indexTable, 0, SEEK_END);
	int indAmount = ftell(indexTable) / sizeof(struct Indexer);

	struct Master master;

	char dummy[51];

	for (int i = 1; i <= indAmount; i++)
	{
		if (getMaster(&master, i, dummy))
		{
			masterCount++;
			slaveCount += master.slavesCount;

			printf("Master #%d has %d slave(s)\n", i, master.slavesCount);
		}
	}

	fclose(indexTable);

	printf("Total masters: %d\n", masterCount);
	printf("Total slaves: %d\n", slaveCount);
}

