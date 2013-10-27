#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/fs.h>

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

typedef struct {
    char    nvh_encoding;   /* nvs encoding method */
    char    nvh_endian;     /* nvs endian */
    char    nvh_reserved1;  /* reserved for future use */
    char    nvh_reserved2;  /* reserved for future use */
} nvs_header_t;

struct uberblock {
    uint64_t        ub_magic;       /* UBERBLOCK_MAGIC              */
    uint64_t        ub_version;     /* SPA_VERSION                  */
    uint64_t        ub_txg;         /* txg of last sync             */
    uint64_t        ub_guid_sum;    /* sum of all vdev guids        */
    uint64_t        ub_timestamp;   /* UTC time of last sync        */
    //blkptr_t        ub_rootbp;      /* MOS objset_phys_t            */
};

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
zio_checksum_SHA256(const void *buf, uint64_t size, uint64_t cksum[]) //, zio_cksum_t *zcp)
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
    printf("<prog> : <devname> <guid> <newguid> <output-device>\n");
}

int hostlittleendian = 1;
int disklittleendian = 1;
void set_endianness(nvs_header_t nvs_header)
{
    disklittleendian = nvs_header.nvh_endian;
    int i = 1;
    char *i_ptr = (char *)&i;
    char c = 1;
    if (i_ptr[0] == c)
        hostlittleendian = 1;
    else
        hostlittleendian = 0;
}

#define LABEL_SIZE 262144       // 256K
#define NVPAIR_OFFSET 16384     // 16k
#define NVPAIR_SIZE 114688      // 112k
#define UBERSTART_OFFSET 131072 // 128K
#define CKSUM_SIZE 32

#define UBERBLOCK_MAGIC         0x00bab10c              /* oo-ba-bloc!  */

int uberblock_size = 1024;      // 1K, this should be calculated as 'ashift' from nvlist

int read_label(std::string path, uint64_t offset, char **buf)
{
    *buf = (char *)malloc(128*1024);
    int fd = open(path.c_str(), O_RDONLY);
    if (fd < 0) {
        printf("Failed to open %s\n", path.c_str());
        return 1;
    }

    bzero(*buf, NVPAIR_SIZE);
    lseek(fd, offset, SEEK_SET);
    int ret = read(fd, *buf, NVPAIR_SIZE-CKSUM_SIZE);
    if (ret != NVPAIR_SIZE-CKSUM_SIZE) {
        printf("%s: Failed to read %d bytes", __FUNCTION__, NVPAIR_SIZE-CKSUM_SIZE);
        close(fd);
        return 1;
    }
    close(fd);

    return 0;
}

int read_uberblock(std::string path, uint64_t offset, char *buf)
{
    int fd = open(path.c_str(), O_RDONLY);
    if (fd < 0) {
        printf("Failed to open %s\n", path.c_str());
        return 1;
    }

    bzero(buf, uberblock_size);
    lseek(fd, offset, SEEK_SET);
    int ret = read(fd, buf, uberblock_size);
    if (ret != uberblock_size) {
        printf("%s: Failed to read %d bytes", __FUNCTION__, uberblock_size);
        close(fd);
        return 1;
    }
    close(fd);

    return 0;
}

int write_label(std::string path, uint64_t offset, char *buf, uint64_t size)
{
    int fd = open(path.c_str(), O_WRONLY);
    if (fd < 0) {
        printf("Failed to open %s\n", path.c_str());
        return 1;
    }

    lseek(fd, offset, SEEK_SET);
    int ret = write(fd, buf, size);
    if (ret != size) {
        printf("%s: Failed to read %d bytes", __FUNCTION__, size);
        close(fd);
        return 1;
    }
    close(fd);

    return 0;
}

void read_nvheader(char *buf, nvs_header_t &header)
{
    memcpy(&header, buf, sizeof(header));
}

#define BSWAP_8(x)      ((x) & 0xff)
#define BSWAP_16(x)     ((BSWAP_8(x) << 8) | BSWAP_8((x) >> 8))
#define BSWAP_32(x)     ((BSWAP_16(x) << 16) | BSWAP_16((x) >> 16))
#define BSWAP_64(x)     ((BSWAP_32(x) << 32) | BSWAP_32((x) >> 32))

uint64_t swap64(uint64_t input)
{
    uint64_t output = input;
#if 0
    char * output_ptr = (char *)&output;
    char c; 
    for (int i = 0; i < 4; i++) {
        char c = output_ptr[i];
        output_ptr[i] = output_ptr[7-i];
        output_ptr[7-i] = c;
    }
#else
    output = BSWAP_64(input);
#endif
    return output;
}

uint64_t _htonll(uint64_t input)
{
    if (!hostlittleendian)
        return input;

    return swap64(input);
}

uint64_t _ntohll(uint64_t input)
{
    return _htonll(input);
}

void writeInt64DiskEndian(char *buf, uint64_t input)
{
    uint64_t inputendian;
    if (disklittleendian != hostlittleendian) {
        //printf("converting endian\n");
        inputendian = swap64(input);
    } else {
        //printf("NOT converting endian\n");
        inputendian = input;
    }

    memcpy(buf, &inputendian, sizeof(inputendian));

    return;
}

uint64_t toInt64HostEndian(uint64_t input)
{
    uint64_t inputendian;
    if (disklittleendian != hostlittleendian) {
        //printf("converting endian\n");
        inputendian = swap64(input);
    } else {
        //printf("NOT converting endian\n");
        inputendian = input;
    }

    return inputendian;
}

uint64_t toInt64DiskEndian(uint64_t input)
{
    return toInt64HostEndian(input);
}

int compute_label_checksum(char *buf, uint64_t compute_offset, uint64_t datasize, bool updateinline)
{
    char savebuf[CKSUM_SIZE];
    memcpy(savebuf, buf+datasize-CKSUM_SIZE, CKSUM_SIZE);

    writeInt64DiskEndian(buf+datasize-32, compute_offset);
    bzero(buf+datasize-32+8, 24); 

    uint64_t cksum[4];
    zio_checksum_SHA256(buf, datasize, cksum);

    if (!updateinline) {
        //printf("Restoring cksum\n");
        memcpy(buf+datasize-CKSUM_SIZE, savebuf, CKSUM_SIZE);
    } else {
        printf("Updating checksum inline\n");
        for (int i = 0; i < 4; i++) {
            printf("cksum %d = %lx\n", i, cksum[i]);
            writeInt64DiskEndian(buf+datasize-CKSUM_SIZE+(i*sizeof(uint64_t)), cksum[i]);
        }        
    }

    return 0;
}

int getDevGeometry(const std::string & devPath, uint64_t &devsize)
{
    struct stat statbuf;
    int err = stat(devPath.c_str(), &statbuf);

    if (err || !S_ISBLK(statbuf.st_mode)) {
        printf("Failed to stat block device to get device geometry of %s\n", devPath.c_str());
#if 0
        return -1;
#else
        devsize = 104857600;
        return 0;
#endif
    }

    int fd = open(devPath.c_str(), O_RDONLY|O_NONBLOCK);
    if ( fd < 0 ) {
        printf("Failed to open device for getting device geometry of %s", devPath.c_str());
        return -1;
    }

    // First try to get the total size in byte, since it is simple
    uint64_t size = 0;
    if (ioctl(fd, BLKGETSIZE64, &size) == 0) {
        printf("size of %s is = %lu", devPath.c_str(), size);
        devsize = size;
        close(fd);
        return 0;
    }

    close(fd);
    printf("Failed to get size of %s", devPath.c_str());
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        Usage();
        return 1;
    }

    std::string device = argv[1];

    uint64_t guid = strtoull(argv[2], NULL, 10);
    uint64_t guid_new = 0;
    bool guid_change = false;
    std::string output_device;
    if (argc > 4) {
        guid_change = true;
        guid_new = strtoull(argv[3], NULL, 10);
        output_device = argv[4];
    }

    printf("dev = %s guid = %lu output_device = %s\n", device.c_str(), guid, output_device.c_str());

    uint64_t size;
    if (getDevGeometry(device, size)) {
        printf("Failed to get size of %s\n", device.c_str());
        return 1;
    }

    char *buf;
    int err = read_label(device, NVPAIR_OFFSET, &buf);
    if (err) {
        printf("%s: Failed to read label, err - %d", __FUNCTION__, err);
        return err;
    }

    nvs_header_t nvs_header;
    read_nvheader(buf, nvs_header);
    set_endianness(nvs_header);

    for (int i = 0; i < 4; i++) {

        uint64_t label_offset;
        uint64_t nvpair_offset;
        if (i == 0)
            label_offset = 0;
        if (i == 1)
            label_offset = 0 + LABEL_SIZE;
        if (i == 2)
            label_offset = size - (2*LABEL_SIZE);
        if (i == 3)
            label_offset = size - LABEL_SIZE;
        nvpair_offset = label_offset+NVPAIR_OFFSET;

        printf("label_offset = %d nvpair_offset = %d\n", label_offset, label_offset+NVPAIR_OFFSET);

        int err = read_label(device, nvpair_offset, &buf);
        if (err) {
            printf("%s: Failed to read label, err - %d", __FUNCTION__, err);
            return err;
        }
        //printf("read label is done\n");

        compute_label_checksum(buf, nvpair_offset, NVPAIR_SIZE, false);

        uint64_t guid_n = _htonll(guid);
        uint64_t guid_new_n = _htonll(guid_new);
        printf("dev = %s guid = %lu guid_n = %lu\n", device.c_str(), guid, guid_n);
        for (int i = 0; i < NVPAIR_SIZE-CKSUM_SIZE; i++) {
            if (strncmp(buf+i, "pool_guid", 9) == 0) {
                printf("Found at offset %d\n", i);
                /* search for another 16 bytes for this guid */
                for (int j = 0; j < 16; j++) {
                    if (memcmp(buf+i+9+j, &guid_n, sizeof(guid_n)) == 0) {
                        printf("new found guid at %d\n", j);
                        if (guid_change) {
                            memcpy(buf+i+9+j, &guid_new_n, sizeof(guid_new_n));
                        }
                    }
                    if (memcmp(buf+i+9+j, &guid, sizeof(guid)) == 0) {
                        printf("old found guid at %d\n", j);
                        exit(1);
                    }
                }
            }
        }
        if (guid_change) {
            compute_label_checksum(buf, nvpair_offset, NVPAIR_SIZE, true);
            printf("Writing new checksum to %s at offset %lu\n", output_device.c_str(), nvpair_offset);
            write_label(output_device, nvpair_offset, buf, NVPAIR_SIZE);
        }

        uint64_t offset = UBERSTART_OFFSET+label_offset;

        char *buf2 = (char *)malloc(uberblock_size);
        uint64_t ubermagic = toInt64DiskEndian((uint64_t)UBERBLOCK_MAGIC);
        char *x = (char *)&ubermagic;
        printf("ubermagic %x %x %x %x %x %x %x %x\n", x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7]);
        while(1) {
            if (offset >= label_offset+LABEL_SIZE)
                break;

            int err = read_uberblock(device, offset, buf2);

            struct uberblock *ub = (struct uberblock *)buf2;
            if (ub->ub_magic  == ubermagic) {
    //        if (memcmp(&ubermagic, buf, sizeof(ubermagic)) == 0) {
                printf("Found ubermagic at %lu %lx\n", offset, offset);
                printf("guid_sum %lu\n", ub->ub_guid_sum);
                if (guid_change) {
                    printf("adding to guid %ld\n", guid_new - guid);
                    uint64_t guid_host_endian = toInt64HostEndian(ub->ub_guid_sum);
                    printf("before guid_host_endian = %lu\n", guid_host_endian);
                    guid_host_endian += guid_new;
                    guid_host_endian -= guid;
                    printf("after guid_host_endian = %lu\n", guid_host_endian);
                    //writeInt64DiskEndian((char *)(&(ub->ub_guid_sum)), guid_host_endian);
                    ub->ub_guid_sum = toInt64DiskEndian(guid_host_endian);
                    printf("guid_sum %lu\n", toInt64HostEndian(ub->ub_guid_sum));
                }

            } else {
                offset += uberblock_size;
                continue;
            }
            
            compute_label_checksum(buf2, offset, uberblock_size, true);

            if (guid_change /* also add check for uberchanged */) {
                printf("Writing new uberblock at offset %lu %lx\n", offset, offset);
                write_label(output_device, offset, buf2, uberblock_size);
            }

            offset += uberblock_size;

        }
    }


    char *outfile = "import.out";
#if 0
    fd = open(outfile, O_CREAT|O_TRUNC|O_WRONLY, 0644);
    if (fd < 0) {
	printf("Failed to open dest file %s", outfile);
	return 1;
    }
    write(fd, buf, 114688);
    close(fd);
    memcpy(buf+114688-32, &cksum, sizeof(cksum));

    int fd;
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
#endif


    return 0;
}

