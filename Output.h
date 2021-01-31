#pragma once

#include <stdio.h>
#include "Master.h"
#include "Struct.h"

void printMaster(struct Master master)
{
	printf("Master\'s name: %s\n", master.name);
	printf("Master\'s status: %d\n", master.status);
}

void printSlave(struct Slave slave, struct Master master)
{
	printf("Master: %s, %d scores\n", master.name, master.status);
	printf("Time: %d\n", slave.time);
}