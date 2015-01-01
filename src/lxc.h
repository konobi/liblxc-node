#include <nan.h>

using namespace v8;

extern "C" {
//#include <stdbool.h>
//
#include <lxc/lxccontainer.h>
#include <lxc/attach_options.h>
#include <lxc/version.h>

//#include <lxc/attach_options.h>
}

# define Con() (ObjectWrap::Unwrap<Container>(args.This()))->con_

class Container : public node::ObjectWrap {
  private:
    struct lxc_container *con_;
    static Persistent<Function> constructor;
 
    Container();
    ~Container();
    static NAN_METHOD(New);
    static NAN_METHOD(Defined);
    static NAN_METHOD(State);
    static NAN_METHOD(Running);
    static NAN_METHOD(Freeze);
    static NAN_METHOD(UnFreeze);
    static NAN_METHOD(InitPid);
    static NAN_METHOD(WantDaemonize);
    static NAN_METHOD(WantCloseAllFds);
    static NAN_METHOD(Create);
    static NAN_METHOD(Start);
    static NAN_METHOD(Stop);
    static NAN_METHOD(Reboot);
    static NAN_METHOD(Shutdown);
    static NAN_METHOD(ConfigFileName);
    static NAN_METHOD(Destroy);
    static NAN_METHOD(Wait);
    static NAN_METHOD(GetConfigItem);
    static NAN_METHOD(SetConfigItem);
    static NAN_METHOD(ClearConfig);
    static NAN_METHOD(ClearConfigItem);
    static NAN_METHOD(GetRunningConfigItem);
    static NAN_METHOD(GetKeys);
    static NAN_METHOD(GetCGroupItem);
    static NAN_METHOD(SetCGroupItem);
    static NAN_METHOD(GetConfigPath);
    static NAN_METHOD(SetConfigPath);
    static NAN_METHOD(LoadConfig);
    static NAN_METHOD(SaveConfig);
    static NAN_METHOD(Clone);
    static NAN_METHOD(ConsoleGetFd);
    static NAN_METHOD(Console);
    static NAN_METHOD(GetInterfaces);
    static NAN_METHOD(GetIps);
    static NAN_METHOD(MayControl);
    static NAN_METHOD(Snapshot);
    static NAN_METHOD(SnapshotList);
    static NAN_METHOD(SnapshotRestore);
    static NAN_METHOD(SnapshotDestroy);
    static NAN_METHOD(AddDeviceNode);
    static NAN_METHOD(RemoveDeviceNode);
    static NAN_METHOD(Rename);
 
  public:
    static void Init(Handle<Object> target, Handle<Object> module);
};

//int lxc_attach(struct lxc_container *c, bool clear_env) {
//	int ret;
//	pid_t pid;
//	lxc_attach_options_t attach_options = LXC_ATTACH_OPTIONS_DEFAULT;

//	attach_options.env_policy = LXC_ATTACH_KEEP_ENV;
//	if (clear_env) {
//		attach_options.env_policy = LXC_ATTACH_CLEAR_ENV;
//	}

	/*
	   remount_sys_proc
	   When using -s and the mount namespace is not included, this flag will cause lxc-attach to remount /proc and /sys to reflect the current other namespace contexts.
	   default_options.attach_flags |= LXC_ATTACH_REMOUNT_PROC_SYS;

	   elevated_privileges
	   Do  not  drop privileges when running command inside the container. If this option is specified, the new process will not be added to the container's cgroup(s) and it will not drop its capabilities before executing.
	   default_options.attach_flags &= ~(LXC_ATTACH_MOVE_TO_CGROUP | LXC_ATTACH_DROP_CAPABILITIES | LXC_ATTACH_APPARMOR);

	   Specify the namespaces to attach to, as a pipe-separated list, e.g. NETWORK|IPC. Allowed values are MOUNT, PID, UTSNAME, IPC, USER and NETWORK.
	   default_options.namespaces = namespace_flags; // lxc_fill_namespace_flags(arg, &namespace_flags);

	   Specify the architecture which the kernel should appear to be running as to the command executed.
	   default_options.personality = new_personality; // lxc_config_parse_arch(arg);

	   Keep the current environment for attached programs.
	   Clear the environment before attaching, so no undesired environment variables leak into the container.

	   default_options.env_policy = env_policy; // LXC_ATTACH_KEEP_ENV or LXC_ATTACH_CLEAR_ENV

	   default_options.extra_env_vars = extra_env;
	   default_options.extra_keep_env = extra_keep;
	*/

//	ret = c->attach(c, lxc_attach_run_shell, NULL, &attach_options, &pid);
//	if (ret < 0)
//		return -1;

//	ret = lxc_wait_for_pid_status(pid);
//	if (ret < 0)
//		return -1;

//	if (WIFEXITED(ret))
//		return WEXITSTATUS(ret);

//	return -1;
//}

/*
int lxc_attach_run_wait(struct lxc_container *c, bool clear_env, int stdinfd, int stdoutfd, int stderrfd, const char * const argv[]) {
	int ret;
	lxc_attach_options_t attach_options = LXC_ATTACH_OPTIONS_DEFAULT;

	attach_options.env_policy = LXC_ATTACH_KEEP_ENV;
	if (clear_env) {
		attach_options.env_policy = LXC_ATTACH_CLEAR_ENV;
	}
	attach_options.stdin_fd = stdinfd;
	attach_options.stdout_fd = stdoutfd;
	attach_options.stderr_fd = stderrfd;

	ret = c->attach_run_wait(c, &attach_options, argv[0], argv);
	if (WIFEXITED(ret) && WEXITSTATUS(ret) == 255)
		return -1;
	return ret;
}
*/


