#include "Struct.h"
#include "Master.h"
#include "Input.h"
#include "Output.h"
#include "Slave.h"

int main()
{
	struct Master master;
	struct Slave slave;

	while (1)
	{
		int choice;
		int id;
		char error[51];

		printf("choose option:\n0 - quit\n1 - insert master\n2 - get master\n3 - update master\n4 - delete master\n5 - insert slave\n6 - get slave\n7 - update slave\n8 - delete slave\n9 - info\n");
		scanf("%d", &choice);

		switch (choice)
		{
		case 0:
			return 0;

		case 1:
			readMaster(&master);
			insertMaster(master);
			break;

		case 2:
			printf("Enter ID: ");
			scanf("%d", &id);
			getMaster(&master, id);
			printMaster(master);
			break;

		case 3:
			printf("Enter ID: ");
			scanf("%d", &id);
			master.id = id;
			readMaster(&master);
			updateMaster(master);
			printf("Updated successfully\n");
			break;

		case 4:
			printf("Enter ID: ");
			scanf("%d", &id);
			deleteMaster(id);
			printf("Deleted successfully\n");
			break;

		case 5:
			printf("Enter master\'s ID: ");
			scanf("%d", &id);
			slave.masterId = id;
			printf("Enter train ID: ");
			scanf("%d", &id);
			slave.trainId = id;
			readSlave(&slave);
			insertSlave(master, slave);
			printf("Inserted successfully. To access, use master\'s and product\'s IDs\n");

			break;

		case 6:
			printf("Enter master\'s ID: ");
			scanf("%d", &id);
			printf("Enter product ID: ");
			scanf("%d", &id);
			getSlave(master, &slave, id);
			printSlave(slave, master);
			break;

		case 7:
			printf("Enter master\'s ID: ");
			scanf("%d", &id);
			printf("Enter product ID: ");
			scanf("%d", &id);
			readSlave(&slave);
			updateSlave(slave, id);
			printf("Updated successfully\n");

			break;

		case 8:
			printf("Enter master\'s ID: ");
			scanf("%d", &id);
			printf("Enter product ID: ");
			scanf("%d", &id);
			deleteSlave(master, slave, id);
			printf("Deleted successfully\n");
			break;

		case 9:
			info();
			break;

		default:
			printf("invalid input, please try again\n");
		}

		printf("---------\n");
	}

	return 0;
}