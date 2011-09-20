#ifndef __NFSD_HOOK_H
#define __NFSD_HOOK_H

#include <linux/version.h>
//#include <linux/sched.h>
#include <linux/kernel.h>
//#include <linux/mm.h>
//#include <linux/string.h>
//#include <linux/stat.h>
#include <linux/errno.h>
//#include <linux/buffer_head.h>
//#include <linux/smp.h>
//#include <linux/smp_lock.h>
//#include <linux/file.h>
//#include <linux/slab.h>
//#include <linux/vmalloc.h>
//#include <linux/poll.h>
#include <linux/list.h>
#include <linux/init.h>
//#include <linux/pagemap.h>
//#include <linux/namei.h>
#include <linux/module.h>
#include <linux/mount.h>
//#include <linux/seq_file.h>
//#include <linux/page-flags.h>
//#include <linux/writeback.h>
//#include <linux/page-flags.h>
//#include <linux/swap.h>
//#include <linux/pagevec.h>
//#include <linux/writeback.h>
#include <linux/statfs.h>
#include <linux/exportfs.h>
//#include <linux/fs_struct.h>
//#include <linux/compat.h>
//#include <linux/kthread.h>

//#include <asm/system.h>
//#include <asm/segment.h>
//#include <asm/mman.h>
//#include <asm/uaccess.h>
//#include <linux/xattr.h>
#include <linux/sunrpc/svc.h>
#include <linux/sunrpc/stats.h>
#include <linux/proc_fs.h>
#include <linux/nfsd/nfsd.h>
#include <linux/nfsd/cache.h>
#include <linux/nfsd/xdr.h>
#include <linux/nfsd/xdr3.h>
#include <linux/nfsd/nfsfh.h>
#include <linux/smp_lock.h>
#include <linux/nfs2.h>
#include <linux/nfs3.h>
#include <net/net_namespace.h>

struct nfsd_hook_proc3 {
	svc_procfunc create;
	svc_procfunc read;
	svc_procfunc write;
	svc_procfunc remove;
	svc_procfunc rename;
	svc_procfunc link;
};

__be32
nfsd_hook_v3_proc_remove(struct svc_rqst *rqstp, struct nfsd3_diropargs *argp,
						  struct nfsd3_attrstat *resp);
__be32
nfsd_hook_v3_proc_create(struct svc_rqst *rqstp, struct nfsd3_diropargs *argp,
						  struct nfsd3_attrstat *resp);
__be32
nfsd_hook_v3_proc_access(struct svc_rqst *rqstp, struct nfsd3_diropargs *argp,
						  struct nfsd3_attrstat *resp);
__be32
nfsd_hook_v3_proc_getattr(struct svc_rqst *rqstp, struct nfsd3_diropargs *argp,
						  struct nfsd3_attrstat *resp);
#endif
