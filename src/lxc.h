#include <nan.h>

using namespace v8;

extern "C" {
//#include <stdbool.h>
//
#include <lxc/lxccontainer.h>
//#include <lxc/attach_options.h>
}

class Container : public node::ObjectWrap {
  private:
    struct lxc_container *con_;
    static Persistent<Function> constructor;
 
    Container();
    ~Container();
    static Handle<Value> New(const Arguments& args);
    static Handle<Value> Defined(const Arguments& args);
    static Handle<Value> State(const Arguments& args);
    static Handle<Value> Running(const Arguments& args);
    static Handle<Value> Freeze(const Arguments& args);
    static Handle<Value> UnFreeze(const Arguments& args);
    static Handle<Value> InitPid(const Arguments& args);
    static Handle<Value> WantDaemonize(const Arguments& args);
    static Handle<Value> WantCloseAllFds(const Arguments& args);
    static Handle<Value> Create(const Arguments& args);
    static Handle<Value> Start(const Arguments& args);
    //static Handle New(const Arguments& args);
  public:
    static void Init(Handle<Object> target);
};

bool lxc_create(struct lxc_container *c, const char *t, const char *bdevtype, int flags, char * const argv[]) {
	return c->create(c, t, bdevtype, NULL, !!(flags & LXC_CREATE_QUIET), argv);
}

bool lxc_start(struct lxc_container *c, int useinit, char * const argv[]) {
	return c->start(c, useinit, argv);
}

//bool lxc_stop(struct lxc_container *c) {
//	return c->stop(c);
//}

//bool lxc_reboot(struct lxc_container *c) {
//	return c->reboot(c);
//}

//bool lxc_shutdown(struct lxc_container *c, int timeout) {
//	return c->shutdown(c, timeout);
//}

//char* lxc_config_file_name(struct lxc_container *c) {
//	return c->config_file_name(c);
//}

//bool lxc_destroy(struct lxc_container *c) {
//	return c->destroy(c);
//}

//bool lxc_wait(struct lxc_container *c, const char *state, int timeout) {
//	return c->wait(c, state, timeout);
//}

//char* lxc_get_config_item(struct lxc_container *c, const char *key) {
//	int len = c->get_config_item(c, key, NULL, 0);
//	if (len <= 0) {
//		return NULL;
//	}

//	char* value = (char*)malloc(sizeof(char)*len + 1);
//	if (c->get_config_item(c, key, value, len + 1) != len) {
//		return NULL;
//	}
//	return value;
//}

//bool lxc_set_config_item(struct lxc_container *c, const char *key, const char *value) {
//	return c->set_config_item(c, key, value);
//}

//void lxc_clear_config(struct lxc_container *c) {
//    c->clear_config(c);
//}

//bool lxc_clear_config_item(struct lxc_container *c, const char *key) {
//	return c->clear_config_item(c, key);
//}

//char* lxc_get_running_config_item(struct lxc_container *c, const char *key) {
//    return c->get_running_config_item(c, key);
//}

//char* lxc_get_keys(struct lxc_container *c, const char *key) {
//	int len = c->get_keys(c, key, NULL, 0);
//	if (len <= 0) {
//		return NULL;
//	}

//	char* value = (char*)malloc(sizeof(char)*len + 1);
//	if (c->get_keys(c, key, value, len + 1) != len) {
//		return NULL;
//	}
//	return value;
//}

//char* lxc_get_cgroup_item(struct lxc_container *c, const char *key) {
//	int len = c->get_cgroup_item(c, key, NULL, 0);
//	if (len <= 0) {
//		return NULL;
//	}

//	char* value = (char*)malloc(sizeof(char)*len + 1);
//	if (c->get_cgroup_item(c, key, value, len + 1) != len) {
//		return NULL;
//	}
//	return value;
//}

//bool lxc_set_cgroup_item(struct lxc_container *c, const char *key, const char *value) {
//	return c->set_cgroup_item(c, key, value);
//}

//const char* lxc_get_config_path(struct lxc_container *c) {
//	return c->get_config_path(c);
//}

//bool lxc_set_config_path(struct lxc_container *c, const char *path) {
//	return c->set_config_path(c, path);
//}

//bool lxc_load_config(struct lxc_container *c, const char *alt_file) {
//	return c->load_config(c, alt_file);
//}

//bool lxc_save_config(struct lxc_container *c, const char *alt_file) {
//	return c->save_config(c, alt_file);
//}

//bool lxc_clone(struct lxc_container *c, const char *newname, int flags, const char *bdevtype) {
//	return c->clone(c, newname, NULL, flags, bdevtype, NULL, 0, NULL) != NULL;
//}

//int lxc_console_getfd(struct lxc_container *c, int ttynum) {
//	int masterfd;

//	if (c->console_getfd(c, &ttynum, &masterfd) < 0) {
//		return -1;
//	}
//	return masterfd;
//}

//bool lxc_console(struct lxc_container *c, int ttynum, int stdinfd, int stdoutfd, int stderrfd, int escape) {

//	if (c->console(c, ttynum, stdinfd, stdoutfd, stderrfd, escape) == 0) {
//		return true;
//	}
//	return false;
//}

//char** get_interfaces(struct lxc_container *c) {
//	return c->get_interfaces(c);
//}

//char** get_ips(struct lxc_container *c, const char *interface, const char *family, int scope) {
//	return c->get_ips(c, interface, family, scope);
//}

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

bool lxc_may_control(struct lxc_container *c) {
	return c->may_control(c);
}

int lxc_snapshot(struct lxc_container *c) {
	return c->snapshot(c, NULL);
}

int lxc_snapshot_list(struct lxc_container *c, struct lxc_snapshot **ret) {
	return c->snapshot_list(c, ret);
}

bool lxc_snapshot_restore(struct lxc_container *c, const char *snapname, const char *newname) {
	return c->snapshot_restore(c, snapname, newname);
}

bool lxc_snapshot_destroy(struct lxc_container *c, const char *snapname) {
	return c->snapshot_destroy(c, snapname);
}

bool lxc_add_device_node(struct lxc_container *c, const char *src_path, const char *dest_path) {
	return c->add_device_node(c, src_path, dest_path);
}

bool lxc_remove_device_node(struct lxc_container *c, const char *src_path, const char *dest_path) {
	return c->remove_device_node(c, src_path, dest_path);
}

bool lxc_rename(struct lxc_container *c, const char *newname) {
	return c->rename(c, newname);
}

*/


