#include <node_os.h>

#include <node.h>
#include <v8.h>

#include "platform.h"

#include <errno.h>
#include <unistd.h>  // gethostname, sysconf
#include <sys/utsname.h>
#include <string.h>

namespace node {

using namespace v8;

static Handle<Value> GetHostname(const Arguments& args) {
  HandleScope scope;
  char s[255];

  if (gethostname(s, 255) < 0) {
    return Undefined();
  }

  return scope.Close(String::New(s));
}

static Handle<Value> GetOSType(const Arguments& args) {
  HandleScope scope;
  char type[256];
  struct utsname info;

  uname(&info);
  strncpy(type, info.sysname, strlen(info.sysname));
  type[strlen(info.sysname)] = 0;

  return scope.Close(String::New(type));
}

static Handle<Value> GetOSRelease(const Arguments& args) {
  HandleScope scope;
  char release[256];
  struct utsname info;

  uname(&info);
  strncpy(release, info.release, strlen(info.release));
  release[strlen(info.release)] = 0;

  return scope.Close(String::New(release));
}

static Handle<Value> GetCPUInfo(const Arguments& args) {
  HandleScope scope;
  Local<Array> cpus;
  int r = Platform::GetCPUInfo(&cpus);

  if (r < 0) {
    return Undefined();
  }

  return scope.Close(cpus);
}

static Handle<Value> GetFreeMemory(const Arguments& args) {
  HandleScope scope;
  double amount = Platform::GetFreeMemory();

  if (amount < 0) {
    return Undefined();
  }

  return scope.Close(Number::New(amount));
}

static Handle<Value> GetTotalMemory(const Arguments& args) {
  HandleScope scope;
  double amount = Platform::GetTotalMemory();

  if (amount < 0) {
    return Undefined();
  }

  return scope.Close(Number::New(amount));
}

static Handle<Value> GetUptime(const Arguments& args) {
  HandleScope scope;
  double uptime = Platform::GetUptime();

  if (uptime < 0) {
    return Undefined();
  }

  return scope.Close(Number::New(uptime));
}

static Handle<Value> GetLoadAvg(const Arguments& args) {
  HandleScope scope;
  Local<Array> loads = Array::New(3);
  int r = Platform::GetLoadAvg(&loads);

  if (r < 0) {
    return Undefined();
  }

  return scope.Close(loads);
}

void OS::Initialize(v8::Handle<v8::Object> target) {
  HandleScope scope;

  NODE_SET_METHOD(target, "getHostname", GetHostname);
  NODE_SET_METHOD(target, "getLoadAvg", GetLoadAvg);
  NODE_SET_METHOD(target, "getUptime", GetUptime);
  NODE_SET_METHOD(target, "getTotalMem", GetTotalMemory);
  NODE_SET_METHOD(target, "getFreeMem", GetFreeMemory);
  NODE_SET_METHOD(target, "getCPUs", GetCPUInfo);
  NODE_SET_METHOD(target, "getOSType", GetOSType);
  NODE_SET_METHOD(target, "getOSRelease", GetOSRelease);
}


}  // namespace node

NODE_MODULE(node_os, node::OS::Initialize);
