#include "nfsd_hook.h"

struct svc_program	*nfsd_prog;

svc_procfunc nfsd_hook_proc3_orig[23] = {
	[NFS3PROC_NULL] = NULL,
	[NFS3PROC_GETATTR] = NULL,
	[NFS3PROC_SETATTR] = NULL,
	[NFS3PROC_LOOKUP] = NULL,
	[NFS3PROC_ACCESS] = NULL,	
	[NFS3PROC_READLINK] = NULL,
	[NFS3PROC_READ] = NULL,
	[NFS3PROC_WRITE] = NULL,
	[NFS3PROC_CREATE] = NULL,
	[NFS3PROC_MKDIR] = NULL,
	[NFS3PROC_SYMLINK] = NULL,
	[NFS3PROC_MKNOD] = NULL,
	[NFS3PROC_REMOVE] = NULL,
	[NFS3PROC_RMDIR] = NULL,
	[NFS3PROC_RENAME] = NULL,
	[NFS3PROC_LINK] = NULL,
	[NFS3PROC_READDIR] = NULL,
	[NFS3PROC_READDIRPLUS] = NULL,
	[NFS3PROC_FSSTAT] = NULL,
	[NFS3PROC_FSINFO] = NULL,
	[NFS3PROC_PATHCONF] = NULL,
	[NFS3PROC_COMMIT] = NULL
};

svc_procfunc nfsd_hook_proc3_custom[23] = {
	[NFS3PROC_NULL] = NULL,
	[NFS3PROC_GETATTR] = (svc_procfunc) nfsd_hook_v3_proc_getattr,
	[NFS3PROC_SETATTR] = NULL,
	[NFS3PROC_LOOKUP] = NULL,
	[NFS3PROC_ACCESS] = (svc_procfunc) nfsd_hook_v3_proc_access,	
	[NFS3PROC_READLINK] = NULL,
	[NFS3PROC_READ] = NULL,
	[NFS3PROC_WRITE] = NULL,
	[NFS3PROC_CREATE] = (svc_procfunc) nfsd_hook_v3_proc_create,
	[NFS3PROC_MKDIR] = NULL,
	[NFS3PROC_SYMLINK] = NULL,
	[NFS3PROC_MKNOD] = NULL,
	[NFS3PROC_REMOVE] = (svc_procfunc) nfsd_hook_v3_proc_remove,
	[NFS3PROC_RMDIR] = NULL,
	[NFS3PROC_RENAME] = NULL,
	[NFS3PROC_LINK] = NULL,
	[NFS3PROC_READDIR] = NULL,
	[NFS3PROC_READDIRPLUS] = NULL,
	[NFS3PROC_FSSTAT] = NULL,
	[NFS3PROC_FSINFO] = NULL,
	[NFS3PROC_PATHCONF] = NULL,
	[NFS3PROC_COMMIT] = NULL
};

void nfsd_hook_print_handle(struct nfsd3_diropargs *argp, struct nfsd3_attrstat *resp)
{
	static char	buf[80];
	struct svc_fh	*fhp = &(argp->fh);
	struct knfsd_fh *fh = &(fhp->fh_handle);
	u32		id[6];

	if (!argp)
		return;
	sprintf(buf, "%d: %08x %08x %08x %08x %08x %08x",
			fh->fh_size,
			fh->fh_base.fh_pad[0],
			fh->fh_base.fh_pad[1],
			fh->fh_base.fh_pad[2],
			fh->fh_base.fh_pad[3],
			fh->fh_base.fh_pad[4],
			fh->fh_base.fh_pad[5]);
	printk(KERN_INFO "handles %s\n", buf);
	printk(KERN_INFO "fsid type %u\n", fh->fh_fsid_type);
	if (fh->fh_fsid_type == FSID_DEV) {
		printk(KERN_INFO "major %u minor %u\n", (fh->fh_fsid[0])>>16, (fh->fh_fsid[0]) & 0xff00);
	}
	if (fh->fh_fsid_type == FSID_UUID16) {
		memcpy(&id, &(fh->fh_fsid), 16);
		printk(KERN_INFO "uuid %08x %08x %08x %08x\n", id[0], id[1], id[2], id[3]);
		if (resp->fh.fh_export) {
			memcpy(&id, resp->fh.fh_export->ex_uuid, 16);
			printk(KERN_INFO "uuid %08x %08x %08x %08x\n", id[0], id[1], id[2], id[3]);
			if (resp->fh.fh_export->ex_pathname) {
				printk(KERN_INFO "pathname %c%c%c%c\n", resp->fh.fh_export->ex_pathname[0], resp->fh.fh_export->ex_pathname[1], resp->fh.fh_export->ex_pathname[2], resp->fh.fh_export->ex_pathname[3]);
			}
		}
	}
	if (fh->fh_fsid_type == FSID_UUID16_INUM) {
		memcpy(&id, &(fh->fh_fsid)+2, 16);
		printk(KERN_INFO "uuid %08x %08x %08x %08x\n", id[0], id[1], id[2], id[3]);
		if (resp->fh.fh_export) {
			memcpy(&id, resp->fh.fh_export->ex_uuid, 16);
			printk(KERN_INFO "uuid %08x %08x %08x %08x\n", id[0], id[1], id[2], id[3]);
			if (resp->fh.fh_export->ex_pathname) {
				printk(KERN_INFO "pathname %c%c%c%c\n", resp->fh.fh_export->ex_pathname[0], resp->fh.fh_export->ex_pathname[1], resp->fh.fh_export->ex_pathname[2], resp->fh.fh_export->ex_pathname[3]);
			}
		}
	}
	if (fh->fh_fsid_type == FSID_NUM) {
		memcpy(&id, &(fh->fh_fsid), 4);
		printk(KERN_INFO "fsid %08x\n", id[0]);
	}

	return;
}

__be32
nfsd_hook_v3_proc_remove(struct svc_rqst *rqstp, struct nfsd3_diropargs *argp,
					  struct nfsd3_attrstat *resp)
{
	__be32	ret = nfserr_dropit;		// in case we have nothing to call

	printk(KERN_INFO "nfsd_hook: REMOVE \n");
	if (nfsd_hook_proc3_orig[NFS3PROC_REMOVE]) {
		printk(KERN_INFO "nfsd_hook: calling %p\n", nfsd_hook_proc3_orig[NFS3PROC_REMOVE]);
		ret = nfsd_hook_proc3_orig[NFS3PROC_REMOVE](rqstp, argp, resp);
	}

	nfsd_hook_print_handle(argp, resp);
	return ret;
}

__be32
nfsd_hook_v3_proc_create(struct svc_rqst *rqstp, struct nfsd3_diropargs *argp,
					  struct nfsd3_attrstat *resp)
{
	__be32	ret = nfserr_dropit;		// in case we have nothing to call

	printk(KERN_INFO "nfsd_hook: CREATE \n");
	if (nfsd_hook_proc3_orig[NFS3PROC_CREATE]) {
		printk(KERN_INFO "nfsd_hook: calling %p\n", nfsd_hook_proc3_orig[NFS3PROC_CREATE]);
		ret = nfsd_hook_proc3_orig[NFS3PROC_CREATE](rqstp, argp, resp);
	}

	nfsd_hook_print_handle(argp, resp);
	return ret;
}

__be32
nfsd_hook_v3_proc_access(struct svc_rqst *rqstp, struct nfsd3_diropargs *argp,
						  struct nfsd3_attrstat *resp)
{
	__be32	ret = nfserr_dropit;		// in case we have nothing to call

	printk(KERN_INFO "nfsd_hook: ACCESS \n");
	if (nfsd_hook_proc3_orig[NFS3PROC_ACCESS]) {
		printk(KERN_INFO "nfsd_hook: calling %p\n", nfsd_hook_proc3_orig[NFS3PROC_ACCESS]);
		ret = nfsd_hook_proc3_orig[NFS3PROC_ACCESS](rqstp, argp, resp);
	}

	return ret;
}

__be32
nfsd_hook_v3_proc_getattr(struct svc_rqst *rqstp, struct nfsd3_diropargs *argp,
						  struct nfsd3_attrstat *resp)
{
	__be32	ret = nfserr_dropit;		// in case we have nothing to call

	printk(KERN_INFO "nfsd_hook: GETATTR \n");
	if (nfsd_hook_proc3_orig[NFS3PROC_GETATTR]) {
		printk(KERN_INFO "nfsd_hook: calling %p\n", nfsd_hook_proc3_orig[NFS3PROC_GETATTR]);
		ret = nfsd_hook_proc3_orig[NFS3PROC_GETATTR](rqstp, argp, resp);
	}

	return ret;
}

int
hook_nfsd_proc3(struct svc_program *pprog)
{
	if (pprog == NULL)
		return 1;

	printk(KERN_INFO "nfsd_hook: Installing hook %p %s\n", pprog, pprog->pg_name);
	nfsd_prog = pprog;

	printk(KERN_INFO "nfsd_hook: pc_func before %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_REMOVE].pc_func);
	nfsd_hook_proc3_orig[NFS3PROC_REMOVE] = nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_REMOVE].pc_func;
	nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_REMOVE].pc_func = nfsd_hook_proc3_custom[NFS3PROC_REMOVE];
	printk(KERN_INFO "nfsd_hook: pc_func after %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_REMOVE].pc_func);
	printk(KERN_INFO "nfsd_hook: pc_func before %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_CREATE].pc_func);
	nfsd_hook_proc3_orig[NFS3PROC_CREATE] = nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_CREATE].pc_func;
	nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_CREATE].pc_func = nfsd_hook_proc3_custom[NFS3PROC_CREATE];
	printk(KERN_INFO "nfsd_hook: pc_func after %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_CREATE].pc_func);
	printk(KERN_INFO "nfsd_hook: pc_func before %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_ACCESS].pc_func);
	nfsd_hook_proc3_orig[NFS3PROC_ACCESS] = nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_ACCESS].pc_func;
	nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_ACCESS].pc_func = nfsd_hook_proc3_custom[NFS3PROC_ACCESS];
	printk(KERN_INFO "nfsd_hook: pc_func after %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_ACCESS].pc_func);
	printk(KERN_INFO "nfsd_hook: pc_func before %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_GETATTR].pc_func);
	nfsd_hook_proc3_orig[NFS3PROC_GETATTR] = nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_GETATTR].pc_func;
	nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_GETATTR].pc_func = nfsd_hook_proc3_custom[NFS3PROC_GETATTR];
	printk(KERN_INFO "nfsd_hook: pc_func after %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_GETATTR].pc_func);

	return 0;	
}

int
unhook_nfsd_proc3(void)
{
	if (nfsd_prog == NULL)
		return 0;

	printk(KERN_INFO "nfsd_hook: Removing hook %s\n", nfsd_prog->pg_name);

	printk(KERN_INFO "nfsd_hook: pc_func before %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_REMOVE].pc_func);
	nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_REMOVE].pc_func = nfsd_hook_proc3_orig[NFS3PROC_REMOVE];
	printk(KERN_INFO "nfsd_hook: pc_func before %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_REMOVE].pc_func);
	printk(KERN_INFO "nfsd_hook: pc_func before %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_CREATE].pc_func);
	nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_CREATE].pc_func = nfsd_hook_proc3_orig[NFS3PROC_CREATE];
	printk(KERN_INFO "nfsd_hook: pc_func before %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_CREATE].pc_func);
	printk(KERN_INFO "nfsd_hook: pc_func before %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_ACCESS].pc_func);
	nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_ACCESS].pc_func = nfsd_hook_proc3_orig[NFS3PROC_ACCESS];
	printk(KERN_INFO "nfsd_hook: pc_func before %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_ACCESS].pc_func);
	printk(KERN_INFO "nfsd_hook: pc_func before %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_GETATTR].pc_func);
	nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_GETATTR].pc_func = nfsd_hook_proc3_orig[NFS3PROC_GETATTR];
	printk(KERN_INFO "nfsd_hook: pc_func before %p\n", nfsd_prog->pg_vers[3]->vs_proc[NFS3PROC_GETATTR].pc_func);

	return 0;	
}

int 
init_nfsd_hook(void)
{
	int			err = 0;
	struct proc_dir_entry	*proc_net = NULL,
				*proc_net_rpc = NULL,
				*proc_net_rpc_nfsd = NULL,
				*t_entry;
	struct svc_stat		*nfsd_svcstats = NULL;
	struct svc_program	*pprog = NULL;
	int			ret = 0;

	proc_net = init_net.proc_net;
	printk(KERN_INFO "Registering nfsd_hook\n");

	for (t_entry = proc_net->subdir; t_entry; t_entry = t_entry->next) {
		if (t_entry->namelen == 3 && 
					memcmp(t_entry->name, "rpc", 3) == 0) {
			proc_net_rpc = t_entry;
			printk(KERN_INFO "nfsd_hook: found proc_net_rpc at %p\n",
								proc_net_rpc);
			break;
		}
	}

	if (!proc_net_rpc) {
		printk(KERN_INFO "nfsd_hook: failed to find proc_net_rpc\n");
		return -1;
	}
	
	for (t_entry = proc_net_rpc->subdir; t_entry; t_entry = t_entry->next) {
		if (t_entry->namelen == 4 && memcmp(t_entry->name, "nfsd", 4) == 0) {
			proc_net_rpc_nfsd = t_entry;
			printk(KERN_INFO "nfsd_hook: found proc_net_rpc_nfsd at %p\n",
							proc_net_rpc_nfsd);
			break;
		}
	}

	if (!proc_net_rpc_nfsd) {
		printk(KERN_INFO "symev_nfsd_hook: failed to hook nfsd(B)\n");
		return -1;
	}

	// now find the nfsd2 and nfsd3 "program" structs, via the
	// svcstats block...
	if (!(nfsd_svcstats = proc_net_rpc_nfsd->data)) {
		printk(KERN_INFO "nfsd_hook: failed to find proc_net_rpc_nfsd\n");
		return -1;
	}
	printk(KERN_INFO "nfsd_hook: svcstats at %p\n", nfsd_svcstats);

	if (!(pprog = nfsd_svcstats->program)) {
		printk(KERN_INFO "nfsd_hook: nfsd program not valid\n");
		return -1;
	}
	printk(KERN_INFO "nfsd_hook: pprog at %p\n", pprog);

	if (pprog->pg_nvers < 3 || !pprog->pg_vers) {
		printk(KERN_INFO "nfsd_hook: nfsd version < 3\n");
		return -1;
	}

	if(hook_nfsd_proc3(pprog)) {
		printk(KERN_INFO "nfsd_hook: failed to hook nfsd_proc3\n");
		return -1;
	}

	printk(KERN_INFO "nfsd_hook: hooked nfsd3 at %p to %p\n", &nfsd_hook_proc3_custom, &nfsd_hook_proc3_orig);
//	nfsd_hook_proc3_custom.remove(NULL, NULL, NULL);
	goto out;

out:
	return err;
}

void 
exit_nfsd_hook(void)
{
	printk(KERN_INFO "Unregistering nfsd_hook\n");
	unhook_nfsd_proc3();

	return;
}

module_init(init_nfsd_hook)
module_exit(exit_nfsd_hook)
