#include <pthread.h>
#include <stdint.h>
#include <sys/types.h>
#include "contrib/heap.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"

#include "computeIPv4MSS.h"
unsigned int computeIPv4MSS(struct CaptureContext *context) {
	return context->settings->mtu - 40; // 20 байтов на заголовок IPv4 + 20 байтов на заголовок TCP
};
