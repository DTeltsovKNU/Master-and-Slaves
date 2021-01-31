#pragma once
#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include "Struct.h"
#include "Master.h"

#define SLAVE_DATA "slave.fl"
#define SLAVE_GARBAGE "slave_garbage.txt"
#define SLAVE_SIZE sizeof(struct Slave)

void reopenDatabase(FILE* database)
{
	fclose(database);
	database = fopen(SLAVE_DATA, "r+b");
}

void linkAddresses(FILE* database, struct Master master, struct Slave slave)
{
	reopenDatabase(database);								

	struct Slave previous;

	fseek(database, master.firstSlaveAddress, SEEK_SET);

	for (int i = 0; i < master.slavesCount; i++)		    
	{
		fread(&previous, SLAVE_SIZE, 1, database);
		fseek(database, previous.nextAddress, SEEK_SET);
	}

	previous.nextAddress = slave.selfAddress;				
	fwrite(&previous, SLAVE_SIZE, 1, database);				
}

void relinkAddresses(FILE* database, struct Slave previous, struct Slave slave, struct Master* master)
{
	if (slave.selfAddress == master->firstSlaveAddress)		
	{
		if (slave.selfAddress == slave.nextAddress)			
		{
			master->firstSlaveAddress = -1;					
		}
		else                                              
		{
			master->firstSlaveAddress = slave.nextAddress; 
		}
	}
	else                                                    
	{
		if (slave.selfAddress == slave.nextAddress)			
		{
			previous.nextAddress = previous.selfAddress;   
		}
		else                                               
		{
			previous.nextAddress = slave.nextAddress;		
		}

		fseek(database, previous.selfAddress, SEEK_SET);	
		fwrite(&previous, SLAVE_SIZE, 1, database);			
	}
}



int insertSlave(struct Master master, struct Slave slave)
{
	slave.exists = 1;

	FILE* database = fopen(SLAVE_DATA, "a+b");


	

	
		fseek(database, 0, SEEK_END);

		int dbSize = ftell(database);

		slave.selfAddress = dbSize;
		slave.nextAddress = dbSize;

	fwrite(&slave, SLAVE_SIZE, 1, database);					

	if (!master.slavesCount)								    
	{
		master.firstSlaveAddress = slave.selfAddress;
	}
	else                                                      
	{
		linkAddresses(database, master, slave);
	}

	fclose(database);											

	master.slavesCount++;										
	updateMaster(master);								

	return 1;
}

int getSlave(struct Master master, struct Slave* slave, int productId)
{
	FILE* database = fopen(SLAVE_DATA, "rb");


	fseek(database, master.firstSlaveAddress, SEEK_SET);		
	fread(slave, SLAVE_SIZE, 1, database);

	for (int i = 0; i < master.slavesCount; i++)				
	{
		if (slave->trainId == productId)						
		{
			fclose(database);
			return 1;
		}

		fseek(database, slave->nextAddress, SEEK_SET);
		fread(slave, SLAVE_SIZE, 1, database);
	}
				
	fclose(database);
	return 0;
}


int updateSlave(struct Slave slave, int productId)
{
	FILE* database = fopen(SLAVE_DATA, "r+b");

	fseek(database, slave.selfAddress, SEEK_SET);
	fwrite(&slave, SLAVE_SIZE, 1, database);
	fclose(database);

	return 1;
}



int deleteSlave(struct Master master, struct Slave slave, int productId)
{
	FILE* database = fopen(SLAVE_DATA, "r+b");
	struct Slave previous;

	fseek(database, master.firstSlaveAddress, SEEK_SET);

	do		                                                   
	{															
		fread(&previous, SLAVE_SIZE, 1, database);
		fseek(database, previous.nextAddress, SEEK_SET);
	} while (previous.nextAddress != slave.selfAddress && slave.selfAddress != master.firstSlaveAddress);

	relinkAddresses(database, previous, slave, &master);				
	slave.exists = 0;											

	fseek(database, slave.selfAddress, SEEK_SET);				
	fwrite(&slave, SLAVE_SIZE, 1, database);					
	fclose(database);

	master.slavesCount--;										
	updateMaster(master);

}