#include <testDataCompiler.h>
#include <debugTools.h>
#include <stdio.h>

char *FTP_jsonMsg = "{                \
 	\"type\":  \"fixedTimePeriod\", \
	\"pin\":   5,                   \
	\"value\": \"high\"             \
}";

char *NUM_jsonMsg = "{                   \
	\"type\":  \"number\",             \
	\"pins\":  [8,9,10,11,12,13,14,15],\
	\"period\": 10                     \
}";

char *SQW_jsonMsg = "{              \
	\"type\":      \"squareWave\",\
	\"pin\":       5,             \
	\"period\":    10,            \
	\"dutyCycle\": 50             \
}";


TEST (T1, testDataCompiler_init) {
	wError err = WERROR_SUCCESS;
	
	// All sub-modules initialization...
	err = testDataCompiler_init();
	ASSERT_EQ (WERROR_SUCCESS, err);

	return;
}


TEST (T2, testDataCompiler_generate) {
	wError err = WERROR_SUCCESS;
	cJSON  *j_fixedTimePeriod = NULL;
	cJSON  *j_number          = NULL;
	cJSON  *j_squareWave      = NULL;
	
	// JSON messages creation...
	j_fixedTimePeriod = cJSON_Parse(FTP_jsonMsg);
	j_number          = cJSON_Parse(NUM_jsonMsg);
	j_squareWave      = cJSON_Parse(SQW_jsonMsg);

	if (j_fixedTimePeriod == NULL || j_number == NULL || j_squareWave == NULL) {
		// ERROR!
		fprintf(stderr, "ERROR! JSON message parsing failed\n");

	} else {
		err = testDataCompiler_generate(j_fixedTimePeriod);
		ASSERT_EQ (WERROR_SUCCESS, err);

		err = testDataCompiler_generate(j_number);
		ASSERT_EQ (WERROR_SUCCESS, err);

		err = testDataCompiler_generate(j_squareWave);
		ASSERT_EQ (WERROR_SUCCESS, err);
	}

	return;
}


int main() {
	T1__testDataCompiler_init();
	T2__testDataCompiler_generate();

	return(0);
}
