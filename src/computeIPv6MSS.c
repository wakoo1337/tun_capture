#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include "CaptureSettings.h"
#include "CaptureContext.h"

#include "computeIPv6MSS.h"
unsigned int computeIPv6MSS(struct CaptureContext *context) {
	return context->settings->mtu - 60; // 40 байтов на заголовок IPv6 + 20 байтов на заголовок TCP
};
