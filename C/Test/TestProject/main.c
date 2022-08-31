#include "eCUCrcTest.h"
#include "eCUCrcDigestTest.h"
#include "eCUCircularQueueTest.h"
#include "eCUDataPackTest.h"
#include "eCUDataUnPackTest.h"
#include "eCUDataPackUnPackTest.h"
#include "eCUByteStufferTest.h"

int main(void);


int main(void)
{
    /* Start testing */
    crc32Test();
    cUCrcDigestTest();
    circularQueueTest();
    dataPackTest();
    dataUnPackTest();
	dataPackUnPackTest();
    byteStufferTest();

    return 0;
}
