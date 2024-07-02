#include "computeIPv4FragmentsCount.h"
unsigned int computeIPv4FragmentsCount(unsigned int size, unsigned int mtu) {
	return (size / (mtu - 20)) + (((size % (mtu - 20)) == 0) ? 0 : 1);
};
