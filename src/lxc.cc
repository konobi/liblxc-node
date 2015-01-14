#include <node.h>
#include <nan.h>
using namespace v8;

extern "C" {
#include <lxc/lxccontainer.h>
#include <lxc/attach_options.h>
#include <lxc/version.h>
}
#include "lxc.h"

Persistent<Function> Container::constructor;
struct lxc_container Container::*con_;

Container::Container() { }
Container::~Container() { }

void Container::Init(Handle<Object> target, Handle<Object> module) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("Container"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  NanSetPrototypeTemplate(tpl, "is_defined", NanNew<FunctionTemplate>(Defined));
  NanSetPrototypeTemplate(tpl, "state", NanNew<FunctionTemplate>(State));
  NanSetPrototypeTemplate(tpl, "is_running", NanNew<FunctionTemplate>(Running));
  NanSetPrototypeTemplate(tpl, "freeze", NanNew<FunctionTemplate>(Freeze));
  NanSetPrototypeTemplate(tpl, "unfreeze", NanNew<FunctionTemplate>(UnFreeze));
  NanSetPrototypeTemplate(tpl, "init_pid", NanNew<FunctionTemplate>(InitPid));
  NanSetPrototypeTemplate(tpl, "want_daemonize", NanNew<FunctionTemplate>(WantDaemonize));
  NanSetPrototypeTemplate(tpl, "want_close_all_fds", NanNew<FunctionTemplate>(WantCloseAllFds));
  NanSetPrototypeTemplate(tpl, "create", NanNew<FunctionTemplate>(Create));
  NanSetPrototypeTemplate(tpl, "start", NanNew<FunctionTemplate>(Start));
  NanSetPrototypeTemplate(tpl, "stop", NanNew<FunctionTemplate>(Stop));
  NanSetPrototypeTemplate(tpl, "reboot", NanNew<FunctionTemplate>(Reboot));
  NanSetPrototypeTemplate(tpl, "shutdown", NanNew<FunctionTemplate>(Shutdown));
  NanSetPrototypeTemplate(tpl, "config_file_name", NanNew<FunctionTemplate>(ConfigFileName));
  NanSetPrototypeTemplate(tpl, "destroy", NanNew<FunctionTemplate>(Destroy));
  NanSetPrototypeTemplate(tpl, "wait", NanNew<FunctionTemplate>(Wait));
  NanSetPrototypeTemplate(tpl, "get_config_item", NanNew<FunctionTemplate>(GetConfigItem));
  NanSetPrototypeTemplate(tpl, "set_config_item", NanNew<FunctionTemplate>(SetConfigItem));
  NanSetPrototypeTemplate(tpl, "clear_config", NanNew<FunctionTemplate>(ClearConfig));
  NanSetPrototypeTemplate(tpl, "clear_config_item", NanNew<FunctionTemplate>(ClearConfigItem));
  NanSetPrototypeTemplate(tpl, "get_running_config_item", NanNew<FunctionTemplate>(GetRunningConfigItem));
  NanSetPrototypeTemplate(tpl, "get_keys", NanNew<FunctionTemplate>(GetKeys));
  NanSetPrototypeTemplate(tpl, "get_cgroup_item", NanNew<FunctionTemplate>(GetCGroupItem));
  NanSetPrototypeTemplate(tpl, "set_cgroup_item", NanNew<FunctionTemplate>(SetCGroupItem));
  NanSetPrototypeTemplate(tpl, "get_config_path", NanNew<FunctionTemplate>(GetConfigPath));
  NanSetPrototypeTemplate(tpl, "set_config_path", NanNew<FunctionTemplate>(SetConfigPath));
  NanSetPrototypeTemplate(tpl, "load_config", NanNew<FunctionTemplate>(LoadConfig));
  NanSetPrototypeTemplate(tpl, "save_config", NanNew<FunctionTemplate>(SaveConfig));
  NanSetPrototypeTemplate(tpl, "clone", NanNew<FunctionTemplate>(Clone));
  NanSetPrototypeTemplate(tpl, "console_getfd", NanNew<FunctionTemplate>(ConsoleGetFd));
  NanSetPrototypeTemplate(tpl, "console", NanNew<FunctionTemplate>(Console));
  NanSetPrototypeTemplate(tpl, "get_interfaces", NanNew<FunctionTemplate>(GetInterfaces));
  NanSetPrototypeTemplate(tpl, "get_ips", NanNew<FunctionTemplate>(GetIps));
  NanSetPrototypeTemplate(tpl, "may_control", NanNew<FunctionTemplate>(MayControl));
  NanSetPrototypeTemplate(tpl, "snapshot", NanNew<FunctionTemplate>(Snapshot));
  NanSetPrototypeTemplate(tpl, "snapshot_list", NanNew<FunctionTemplate>(SnapshotList));
  NanSetPrototypeTemplate(tpl, "snapshot_restore", NanNew<FunctionTemplate>(SnapshotRestore));
  NanSetPrototypeTemplate(tpl, "snapshot_destroy", NanNew<FunctionTemplate>(SnapshotDestroy));
  NanSetPrototypeTemplate(tpl, "add_device_node", NanNew<FunctionTemplate>(AddDeviceNode));
  NanSetPrototypeTemplate(tpl, "remove_device_node", NanNew<FunctionTemplate>(RemoveDeviceNode));
  NanSetPrototypeTemplate(tpl, "rename", NanNew<FunctionTemplate>(Rename));

  NanAssignPersistent(constructor, tpl->GetFunction());
  //target->Set(NanNew("Container"), tpl->GetFunction());
  module->Set(NanNew("exports"), tpl->GetFunction());
}

NAN_METHOD(Container::New) {
  NanScope();
  static v8::String *cname;
  static v8::String *configpath;

  if (args.IsConstructCall()) {
    cname = *args[0].As<String>();
    configpath = *args[1].As<String>();
    Container* obj = new Container();

    obj->con_ = lxc_container_new(
      (const char*)cname,
      (const char*)configpath
    );

    if(obj->con_ == NULL) {
        NanThrowError("Unable to get container");
      NanReturnUndefined();
    }

    obj->Wrap(args.This());
    return args.This();
  } else {
    NanReturnUndefined();
  }
}

NAN_METHOD(Container::Defined) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->is_defined(obj->con_) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::State) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return NanNew(std::string(obj->con_->state(obj->con_)));
}

NAN_METHOD(Container::Running) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->is_running(obj->con_) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::Freeze) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->freeze(obj->con_) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::UnFreeze) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->unfreeze(obj->con_) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::InitPid) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  NanReturnValue(NanNew<Number>(obj->con_->init_pid(obj->con_)));
}

NAN_METHOD(Container::WantDaemonize) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->want_daemonize(obj->con_, args[0]->ToBoolean()->Value()) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::WantCloseAllFds) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->want_close_all_fds(obj->con_, args[0]->ToBoolean()->Value()) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::Create) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 

  Handle<String> template_str = args[0].As<String>();
  Handle<String> bdevtype_str = args[1].As<String>();
  Handle<Integer> flags_num = args[2].As<Integer>();

  char * extra[16];

  return obj->con_->create(
    obj->con_,
    **(new NanUtf8String(template_str)),
    **(new NanUtf8String(bdevtype_str)),
    NULL,
    !!((int64_t)*flags_num & LXC_CREATE_QUIET),
    extra
  ) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::Start) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  char * extra[16];
  return obj->con_->start(
      obj->con_,
      args[0]->ToNumber()->Value(),
      extra
  ) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::Stop) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->stop(obj->con_) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::Reboot) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->reboot(obj->con_) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::Shutdown) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  Handle<Integer> timeout = args[0].As<Integer>();
  return obj->con_->shutdown(obj->con_, (int64_t)*timeout) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::ConfigFileName) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This());
  NanReturnValue(NanNew(obj->con_->config_file_name(obj->con_)));
}

NAN_METHOD(Container::Destroy) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->destroy(obj->con_) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::Wait) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 

  Handle<String> state = args[0].As<String>();
  Handle<Integer> timeout = args[1].As<Integer>();

  return obj->con_->wait(obj->con_, **(new NanUtf8String(state)), (int64_t)*timeout) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::GetConfigItem) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This());
  struct lxc_container *c = obj->con_;
  char * key = **(new NanUtf8String(args[0].As<String>()));
  int len = c->get_config_item(c, key, NULL, 0);
  if (len <= 0) {
    NanReturnNull();
  }

  char* value = (char*)malloc(sizeof(char)*len + 1);
  if (c->get_config_item(c, key, value, len + 1) != len) {
    NanReturnNull();
  }
  // XXX - need to free here!
  NanReturnValue(NanNew(value));
}

NAN_METHOD(Container::SetConfigItem) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 

  char * key = **(new NanUtf8String(args[0].As<String>()));
  char * value = **(new NanUtf8String(args[1].As<String>()));

  return obj->con_->set_config_item(obj->con_, key, value) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::ClearConfig) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This());
  obj->con_->clear_config(obj->con_);
  NanReturnUndefined();
}

NAN_METHOD(Container::ClearConfigItem) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  char * key = **(new NanUtf8String(args[0].As<String>()));

  return obj->con_->clear_config_item(obj->con_, key) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::GetRunningConfigItem) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  char * key = **(new NanUtf8String(args[0].As<String>()));

  NanReturnValue(NanNew(obj->con_->clear_config_item(obj->con_, key)));
}

NAN_METHOD(Container::GetKeys) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This());
  struct lxc_container *c = obj->con_;
  char * key = **(new NanUtf8String(args[0].As<String>()));
  int len = c->get_keys(c, key, NULL, 0);
  if (len <= 0) {
    NanReturnNull();
  }

  char* value = (char*)malloc(sizeof(char)*len + 1);
  if (c->get_keys(c, key, value, len + 1) != len) {
    NanReturnNull();
  }
  // XXX - need to free here!
  NanReturnValue(NanNew(value));
}

NAN_METHOD(Container::GetCGroupItem) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This());
  struct lxc_container *c = obj->con_;
  char * key = **(new NanUtf8String(args[0].As<String>()));
  int len = c->get_cgroup_item(c, key, NULL, 0);
  if (len <= 0) {
    NanReturnNull();
  }

  char* value = (char*)malloc(sizeof(char)*len + 1);
  if (c->get_cgroup_item(c, key, value, len + 1) != len) {
    NanReturnNull();
  }
  // XXX - need to free here!
  NanReturnValue(NanNew(value));
}

NAN_METHOD(Container::SetCGroupItem) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  char * key = **(new NanUtf8String(args[0].As<String>()));
  char * value = **(new NanUtf8String(args[1].As<String>()));

  return obj->con_->set_cgroup_item(obj->con_, key, value) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::GetConfigPath) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This());
  NanReturnValue(NanNew(obj->con_->get_config_path(obj->con_)));
}

NAN_METHOD(Container::SetConfigPath) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  char * path = **(new NanUtf8String(args[0].As<String>()));

  return obj->con_->set_config_path(obj->con_, path) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::LoadConfig) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  char * path = **(new NanUtf8String(args[0].As<String>()));

  return obj->con_->load_config(obj->con_, path) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::SaveConfig) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  char * path = **(new NanUtf8String(args[0].As<String>()));

  return obj->con_->save_config(obj->con_, path) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::Clone) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  char * newname = **(new NanUtf8String(args[0].As<String>()));
  Handle<Integer> flags = args[1].As<Integer>();
  char * bdevtype = **(new NanUtf8String(args[2].As<String>()));

  return (obj->con_->clone(
    obj->con_, 
    newname,
    NULL,
    (int64_t)*flags,
    bdevtype,
    NULL,
    0,
    NULL
  ) != NULL) ? NanTrue() : NanFalse();

}

NAN_METHOD(Container::ConsoleGetFd) {
  NanScope();

  int masterfd;

  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  int ttynum = (int64_t)*(args[0].As<Integer>());

  if(obj->con_->console_getfd(obj->con_, &ttynum, &masterfd) < 0) {
    NanReturnValue(NanNew(-1));
  }
  NanReturnValue(NanNew(masterfd));
}

NAN_METHOD(Container::Console) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  int ttynum = (int64_t)*(args[0].As<Integer>());
  int stdinfd = (int64_t)*(args[1].As<Integer>());
  int stdoutfd = (int64_t)*(args[2].As<Integer>());
  int stderrfd = (int64_t)*(args[3].As<Integer>());
  int escape = (int64_t)*(args[4].As<Integer>());

  if(obj->con_->console(obj->con_, ttynum, stdinfd, stdoutfd, stderrfd, escape) == 0) {
    NanReturnValue(NanTrue());
  }

  NanReturnValue(NanFalse());
}

NAN_METHOD(Container::GetInterfaces) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 

  char** interfaces = obj->con_->get_interfaces(obj->con_);
  if(interfaces == NULL) {
    NanReturnUndefined();
  }

  Handle<Array> arr = NanNew<Array>();
  int i = 0;
  while(interfaces++ != '\0'){
    arr->Set(i, NanNew<String>(*interfaces));
  }

  NanReturnValue(arr);
}

NAN_METHOD(Container::GetIps) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  char * interface = **(new NanUtf8String(args[0].As<String>()));
  char * family = **(new NanUtf8String(args[1].As<String>()));
  int linkscope = (int64_t)*(args[2].As<Integer>());

  char** ips = obj->con_->get_ips(obj->con_, interface, family, linkscope);
  if(ips == NULL) {
    NanReturnUndefined();
  }

  Handle<Array> arr = NanNew<Array>();
  int i = 0;
  while(ips++ != '\0'){
    arr->Set(i, NanNew<String>(*ips));
  }

  NanReturnValue(arr);
}

NAN_METHOD(Container::MayControl){
  NanScope();
  return Con()->may_control(Con()) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::Snapshot){
  NanScope();
  NanReturnValue(NanNew<Integer>(Con()->snapshot(Con(), NULL)));
}

// XXX - do stuff with ret rather than just return an int
NAN_METHOD(Container::SnapshotList) {
  NanScope();
  struct lxc_snapshot **ret = {};
  Handle<Object> hash = NanNew<Object>();
  int rval = Con()->snapshot_list(Con(), ret);
  if(rval < 0){
    NanReturnUndefined();
  }
  while(ret++ != '\0'){    
    Handle<Object> snap = NanNew<Object>();
    snap->Set(NanNew("name"), NanNew((**ret).name));
    snap->Set(NanNew("comment_pathname"), NanNew((**ret).comment_pathname));
    snap->Set(NanNew("timestamp"), NanNew((**ret).timestamp));
    snap->Set(NanNew("lxcpath"), NanNew((**ret).lxcpath));

    hash->Set(NanNew((**ret).name), snap);
  }
  NanReturnValue(hash);
}

NAN_METHOD(Container::SnapshotRestore) {
  NanScope();
  char * snapname = **(new NanUtf8String(args[0].As<String>()));
  char * newname = **(new NanUtf8String(args[1].As<String>()));
  return Con()->snapshot_restore(Con(), snapname, newname) ? NanTrue() : NanFalse();

}

NAN_METHOD(Container::SnapshotDestroy) {
  NanScope();
  char * snapname = **(new NanUtf8String(args[0].As<String>()));
  return Con()->snapshot_destroy(Con(), snapname) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::AddDeviceNode) {
  NanScope();
  char * src_path = **(new NanUtf8String(args[0].As<String>()));
  char * dest_path = **(new NanUtf8String(args[1].As<String>()));
  return Con()->add_device_node(Con(), src_path, dest_path) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::RemoveDeviceNode) {
  NanScope();
  char * src_path = **(new NanUtf8String(args[0].As<String>()));
  char * dest_path = **(new NanUtf8String(args[1].As<String>()));
  return Con()->remove_device_node(Con(), src_path, dest_path) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::Rename) {
  NanScope();
  char * newname = **(new NanUtf8String(args[0].As<String>()));
  return Con()->rename(Con(), newname) ? NanTrue() : NanFalse();
}


NODE_MODULE(lxc, Container::Init)
