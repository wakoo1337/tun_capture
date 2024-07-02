#include <stdint.h>
#include "IPFragmentMetadata.h"

#include "fillIPv4FragmentMetadatas.h"
void fillIPv4FragmentMetadatas(struct IPFragmentMetadata *metadata, unsigned int count, unsigned int len, const unsigned int mtu) {
	unsigned int offset = 0;
	for (unsigned int i=0;i < count;i++) {
		const unsigned int max_size = mtu - 20;
		metadata[i].header_size = 20; // Заголовок IPv4 всегда 20 байтов
		metadata[i].fragment_offset = offset;
		if (len > max_size) {
			metadata[i].data_size = max_size & (-8);
			offset += metadata[i].data_size;
			len -= max_size;
		} else {
			metadata[i].data_size = len;
			offset += len;
			len = 0;
		};
	};
};
