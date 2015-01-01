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

void Container::Init(Handle<Object> target) {
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

  NanAssignPersistent(constructor, tpl->GetFunction());
  target->Set(NanNew("Container"), tpl->GetFunction());
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

NODE_MODULE(lxc, Container::Init)
