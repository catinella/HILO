/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//                                              (https://github.com/catinella/HILO)
//
//
// Filename: test_ringBuffer.c
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	
//
------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <minute.h>
#include <ringBuffer.h>

#define PROCDATA_SIZE 8192
#define STORAGE_SIZE  16
#define FOODATA_MAX   128
#define TEST_FILE     "/tmp/test_ringBuffer-T0.csv"

static bitConf_t storage[STORAGE_SIZE];

bitConf_t foodata_get () {
	static bitConf_t foodata = 0;
	static bool      inc     = true;

	if (foodata == 0) {
		inc = true;
		
	} else if (foodata == FOODATA_MAX) {
		inc = false;
	
	} 

	if (inc)
		foodata++;

	else
		foodata--;

	return(foodata);
}

static void refillCB (ringBuffer_t *obj) {
	unsigned int offs = (obj->index >= STORAGE_SIZE/2) ? 0 : (STORAGE_SIZE/2);
	for (unsigned int t = 0; t < (STORAGE_SIZE/2); t++) 
		obj->storage[(t + offs)] = foodata_get();

	return;
}

//------------------------------------------------------------------------------------------------------------------------------
//                                                    T E S T S
//------------------------------------------------------------------------------------------------------------------------------

TEST(ringBuffer, T1) {
	ringBuffer_t inputBuffer;
	bool         rc = false;
	bitConf_t    data = 0;

	//
	// foo-data loading...
	//
	{
		bitConf_t tmp = 0;
		for (unsigned int t = 0; t < STORAGE_SIZE; t++) {
			tmp = foodata_get();
			storage[t] = tmp;
			printf("%d) %d\n", t, tmp);
		}
	}

	// Initialization (illegal argument error)
	rc = ringBuffer_init(&inputBuffer, NULL, STORAGE_SIZE, RINGBUFFER_INBUFF, refillCB);
	ASSERT_TRUE(!rc);
	rc = ringBuffer_init(&inputBuffer, storage, (STORAGE_SIZE - 1), RINGBUFFER_INBUFF, refillCB);
	ASSERT_TRUE(!rc);

	// Initialization (success)
	rc = ringBuffer_init(&inputBuffer, storage, STORAGE_SIZE, RINGBUFFER_INBUFF, refillCB);
	ASSERT_TRUE(rc);

	{
		FILE *fh = fopen(TEST_FILE, "w");
		if (fh == NULL) {
			// ERROR!
		
		} else {
			for (unsigned int t = 0; t < PROCDATA_SIZE; t++) {
				if (ringBuffer_pushPull(&inputBuffer, &data)) {
					fprintf(fh, "%d\n", data);
					fflush(fh);
				} else {
					// ERROR!
					break;
				}
			}
			fclose(fh);
		}
	}
	return;
}


#include <test_ringBuffer.sgc>
