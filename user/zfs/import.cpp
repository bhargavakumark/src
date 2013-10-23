#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>

/*
 * SHA-256 checksum, as specified in FIPS 180-3, available at:
 * http://csrc.nist.gov/publications/PubsFIPS.html
 *
 * This is a very compact implementation of SHA-256.
 * It is designed to be simple and portable, not to be fast.
 */

/*
 * The literal definitions of Ch() and Maj() according to FIPS 180-3 are:
 *
 * 	Ch(x, y, z)     (x & y) ^ (~x & z)
 * 	Maj(x, y, z)    (x & y) ^ (x & z) ^ (y & z)
 *
 * We use equivalent logical reductions here that require one less op.
 */


#define	Ch(x, y, z)	((z) ^ ((x) & ((y) ^ (z))))
#define	Maj(x, y, z)	(((x) & (y)) ^ ((z) & ((x) ^ (y))))
#define	Rot32(x, s)	(((x) >> s) | ((x) << (32 - s)))
#define	SIGMA0(x)	(Rot32(x, 2) ^ Rot32(x, 13) ^ Rot32(x, 22))
#define	SIGMA1(x)	(Rot32(x, 6) ^ Rot32(x, 11) ^ Rot32(x, 25))
#define	sigma0(x)	(Rot32(x, 7) ^ Rot32(x, 18) ^ ((x) >> 3))
#define	sigma1(x)	(Rot32(x, 17) ^ Rot32(x, 19) ^ ((x) >> 10))

static const uint32_t SHA256_K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static void
SHA256Transform(uint32_t *H, const uint8_t *cp)
{
	uint32_t a, b, c, d, e, f, g, h, t, T1, T2, W[64];

	for (t = 0; t < 16; t++, cp += 4)
		W[t] = (cp[0] << 24) | (cp[1] << 16) | (cp[2] << 8) | cp[3];

	for (t = 16; t < 64; t++)
		W[t] = sigma1(W[t - 2]) + W[t - 7] +
		    sigma0(W[t - 15]) + W[t - 16];

	a = H[0]; b = H[1]; c = H[2]; d = H[3];
	e = H[4]; f = H[5]; g = H[6]; h = H[7];

	for (t = 0; t < 64; t++) {
		T1 = h + SIGMA1(e) + Ch(e, f, g) + SHA256_K[t] + W[t];
		T2 = SIGMA0(a) + Maj(a, b, c);
		h = g; g = f; f = e; e = d + T1;
		d = c; c = b; b = a; a = T1 + T2;
	}

	H[0] += a; H[1] += b; H[2] += c; H[3] += d;
	H[4] += e; H[5] += f; H[6] += g; H[7] += h;
}

void
zio_checksum_SHA256(const void *buf, uint64_t size, int64_t cksum[]) //, zio_cksum_t *zcp)
{
	uint32_t H[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
	uint8_t pad[128];
	int i, padsize;

	for (i = 0; i < (size & ~63ULL); i += 64)
		SHA256Transform(H, (uint8_t *)buf + i);

	for (padsize = 0; i < size; i++)
		pad[padsize++] = *((uint8_t *)buf + i);

	for (pad[padsize++] = 0x80; (padsize & 63) != 56; padsize++)
		pad[padsize] = 0;

	for (i = 56; i >= 0; i -= 8)
		pad[padsize++] = (size << 3) >> i;

	for (i = 0; i < padsize; i += 64)
		SHA256Transform(H, pad + i);

        for (i = 0; i < 7; i = i+2) {
            printf("H[%d] = %08x%08x\n", i, H[i], H[i+1]);
        }

        cksum[0] = (uint64_t)H[0] << 32 | H[1];
        cksum[1] = (uint64_t)H[2] << 32 | H[3];
        cksum[2] = (uint64_t)H[4] << 32 | H[5];
        cksum[3] = (uint64_t)H[6] << 32 | H[7];
        /*
	ZIO_SET_CHECKSUM(zcp,
	    (uint64_t)H[0] << 32 | H[1],
	    (uint64_t)H[2] << 32 | H[3],
	    (uint64_t)H[4] << 32 | H[5],
	    (uint64_t)H[6] << 32 | H[7]);
        */
}

int Usage()
{
    printf("<prog> : <devname> <offset> <guid> <output-device>\n");
}

int main(int argc, char *argv[])
{
    if (argc < 4) {
        Usage();
        return 1;
    }

    std::string device = argv[1];

    uint64_t offset = strtoull(argv[2], NULL, 10);
    uint64_t guid = strtoull(argv[3], NULL, 10);
    std::string output_device;
    if (argc > 4)
	    output_device = argv[4];
    printf("dev = %s offset = %lu guid = %lu output_device = %s\n", device.c_str(), offset, guid, output_device.c_str());


    char *buf = (char *)malloc(128*1024);
    int fd = open(device.c_str(), O_RDONLY);
    if (fd < 0) {
        printf("Failed to open %s\n", device.c_str());
        return 1;
    }
    bzero(buf, 128*1024);
    lseek(fd, offset, SEEK_SET);
    int ret = read(fd, buf, 114688-32);
    if (ret != 114688-32) {
        printf("Failed to read %d bytes", 114688-32);
        return 1;
    }
    close(fd);

    int64_t guid_n = guid;
    int64_t guid_new = guid_n + 1;
    char * guid_ptr = (char *)&guid_n;
    char c; 
    for (int i = 0; i < 4; i++) {
        char c = guid_ptr[i];
        guid_ptr[i] = guid_ptr[7-i];
        guid_ptr[7-i] = c;
    }
    printf("dev = %s guid = %lu\n", device.c_str(), guid);
    for (int i = 0; i < 114688-32; i++) {
        if (strncmp(buf+i, "pool_guid", 9) == 0) {
            printf("Found at offset %d\n", i);
            /* search for another 16 bytes for this guid */
            for (int j = 0; j < 16; j++) {
		if (memcmp(buf+i+9+j, &guid_n, sizeof(guid_n)) == 0) {
                    printf("new found guid at %d\n", j);
                    memcpy(buf+i+9+j, &guid_new, sizeof(guid_new));
                }
		if (memcmp(buf+i+9+j, &guid, sizeof(guid)) == 0) {
                    printf("new found guid at %d\n", j);
                    memcpy(buf+i+9+j, &guid_new, sizeof(guid_new));
                }
            }
        }
    }
		 
    memcpy(buf+114688-32, &offset, sizeof(offset));
    bzero(buf+114688-32+8, 24); 

    int64_t cksum[4];
    zio_checksum_SHA256(buf, 114688, cksum);

    char *outfile = "import.out";
#if 0
    fd = open(outfile, O_CREAT|O_TRUNC|O_WRONLY, 0644);
    if (fd < 0) {
	printf("Failed to open dest file %s", outfile);
	return 1;
    }
    write(fd, buf, 114688);
    close(fd);
#endif
    memcpy(buf+114688-32, &cksum, sizeof(cksum));

    if (output_device.empty()) {
	    outfile = "import.out.new";
	    fd = open(outfile, O_CREAT|O_TRUNC|O_WRONLY, 0644);
	    if (fd < 0) {
		printf("Failed to open dest file %s", outfile);
		return 1;
	    }
    } else {
	    fd = open(output_device.c_str(), O_WRONLY);
	    if (fd < 0) {
		printf("Failed to open dest file %s", output_device.c_str());
		return 1;
	    }
    	    lseek(fd, offset, SEEK_SET);
    }

    write(fd, buf, 114688);
    close(fd);

    return 0;
}
